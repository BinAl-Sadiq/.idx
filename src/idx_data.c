#define _CRT_SECURE_NO_WARNINGS

#include "idx_data.h"

#include "idx_swap.h"
#include "idx_data_type.h"

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdarg.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

extern enum idx_error_type idx_error;

void idx_data_free(struct idx_data* idx)
{
	free(idx->data);
	free(idx->dimensions_lengths);
	free(idx->dimensions_elements_sizes);
	free(idx);
}

struct idx_data* idx_load_from_idx_file(const char* file_name)
{
	struct idx_data* idx = malloc(sizeof(struct idx_data));
	if (!idx)
	{
		idx_error = IDX_MEMORY_SHORTAGE;
		return 0;
	}
	*idx = (struct idx_data){0};

	FILE* fp = fopen(file_name, "rb");
	if (!fp)
	{
		idx_error = IDX_COULD_NOT_OPEN_FILE;
		free(idx);
		return 0;
	}

	fread(&idx->first_two_bytes, 2, 1, fp);
	if (idx->first_two_bytes)
	{
		idx_error = IDX_INVALID_FIRST_TWO_BYTES;
		fclose(fp);
		free(idx);
		return 0;
	}
	
	fread(&idx->data_type, 1, 1, fp);
	size_t data_type_size = idx_data_type_size(idx->data_type);
	if ( !data_type_size)
	{
		idx_error = IDX_INVALID_DATA_TYPE;
		fclose(fp);
		free(idx);
		return 0;
	}

	fread(&idx->dimensions_count, 1, 1, fp);
	if (!idx->dimensions_count)
	{
		idx_error = IDX_NO_DIMENSIONS;
		fclose(fp);
		free(idx);
		return 0;
	}

	idx->dimensions_lengths = malloc(idx->dimensions_count * sizeof(uint32_t));
	if (!idx->dimensions_lengths)
	{
		idx_error = IDX_MEMORY_SHORTAGE;
		fclose(fp);
		free(idx);
		return 0;
	}
	for (uint8_t i = 0; idx->dimensions_count > i; i++)
	{
		fread(&idx->dimensions_lengths[i], sizeof(uint32_t), 1, fp);
		idx->dimensions_lengths[i] = *(uint32_t*)idx_swap32(&idx->dimensions_lengths[i]);
	}

	idx->dimensions_elements_sizes = malloc(idx->dimensions_count * sizeof(size_t));
	if (!idx->dimensions_elements_sizes)
	{
		idx_error = IDX_MEMORY_SHORTAGE;
		fclose(fp);
		free(idx->dimensions_lengths);
		free(idx);
		return 0;
	}
	size_t comulative_size = data_type_size;
	for (int i = idx->dimensions_count-1; i > -1; i--)
	{
		idx->dimensions_elements_sizes[i] = comulative_size;
		comulative_size *= idx->dimensions_lengths[i];
	}

	size_t data_size = ftell(fp);
	fseek(fp, 0, SEEK_END);
	data_size = ftell(fp) - data_size;
	idx->data = malloc(data_size);
	if (!idx->data)
	{
		idx_error = IDX_MEMORY_SHORTAGE;
		fclose(fp);
		free(idx->dimensions_lengths);
		free(idx->dimensions_elements_sizes);
		free(idx);
		return 0;
	}
	fseek(fp, 4 + idx->dimensions_count * sizeof(uint32_t), SEEK_SET);
	fread(idx->data, sizeof(uint8_t), data_size, fp);
	fclose(fp);

	return idx;
}

struct idx_data* idx_load_from_image_file(const char* file_name)
{
	int width, height, channels;
	uint8_t* img = stbi_load(file_name, &width, &height, &channels, 0);
	if (!img)
	{
		idx_error = IDX_COULD_NOT_LOAD_IMAGE;
		return 0;
	}

	struct idx_data* idx = malloc(sizeof(struct idx_data));
	if (!idx)
	{
		idx_error = IDX_MEMORY_SHORTAGE;
		stbi_image_free(img);
		return 0;
	}

	uint32_t* dimensions_lengths = malloc(3 * sizeof(uint32_t));
	if (!dimensions_lengths)
	{
		idx_error = IDX_MEMORY_SHORTAGE;
		stbi_image_free(img);
		free(idx);
		return 0;
	}
	dimensions_lengths[0] = width;
	dimensions_lengths[1] = height;
	dimensions_lengths[2] = channels;

	size_t* dimensions_elements_sizes = malloc(3 * sizeof(size_t));
	if (!dimensions_elements_sizes)
	{
		idx_error = IDX_MEMORY_SHORTAGE;
		stbi_image_free(img);
		free(idx);
		free(dimensions_lengths);
		return 0;
	}
	dimensions_elements_sizes[2] = idx_data_type_size(IDX_UINT8);
	dimensions_elements_sizes[1] = dimensions_elements_sizes[2] * dimensions_lengths[2];
	dimensions_elements_sizes[0] = dimensions_elements_sizes[1] * dimensions_lengths[1];

	*idx = (struct idx_data)
	{ 
		//magic number:
		0,
		IDX_UINT8,
		3,

		dimensions_lengths,
		dimensions_elements_sizes,
		img
	};

	return idx;
}

struct idx_data* idx_load_from_png_file(const char* file_name)
{
	return idx_load_from_image_file(file_name);
}

struct idx_data* idx_load_from_jpg_file(const char* file_name)
{
	return idx_load_from_image_file(file_name);
}

void idx_store_to_idx_file(const struct idx_data* idx, const char* file_name)
{
	FILE* fp = fopen(file_name, "wb");
	if (!fp)
	{
		idx_error = IDX_COULD_NOT_OPEN_FILE;
		return;
	}

	fwrite(idx, 4, 1, fp);//write the magic number

	void* bigendian_dimensions_lengths = idx_swap_array(idx->dimensions_lengths, idx->dimensions_count, IDX_SINT32);
	if (!bigendian_dimensions_lengths)
	{
		fclose(fp);
		return;
	}
	fwrite(bigendian_dimensions_lengths, sizeof(uint32_t), idx->dimensions_count, fp);
	free(bigendian_dimensions_lengths);

	fwrite(idx->data, idx_data_type_size(idx->data_type), idx_get_elements_count(idx), fp);

	fclose(fp);
}

void idx_store_to_png_file(const struct idx_data* idx, const char* file_name)
{
	stbi_write_png(file_name, idx->dimensions_lengths[0], idx->dimensions_lengths[1], idx->dimensions_lengths[2], idx->data, idx->dimensions_lengths[0] * idx->dimensions_lengths[2]);
}

void idx_store_to_jpg_file(const struct idx_data* idx, const char* file_name)
{
	stbi_write_jpg(file_name, idx->dimensions_lengths[0], idx->dimensions_lengths[1], idx->dimensions_lengths[2], idx->data, 100);
}

void* idx_get_element_ptr(struct idx_data* idx, ...)
{
	va_list vargs;
	va_start(vargs, idx);
	size_t element_index = 0;
	for (uint8_t i = 0; i < idx->dimensions_count; i++)
		element_index += idx->dimensions_elements_sizes[i] * va_arg(vargs, uint32_t);
	va_end(vargs);

	return idx->data + element_index;
}
