#pragma once

#include <stdint.h>

#include "idx_error.h"
#include "idx_data_type.h"

struct idx_data
{
	uint16_t first_two_bytes;//must equal 0
	uint8_t data_type;
	uint8_t dimensions_count;
	uint32_t* dimensions_lengths;
	size_t* dimensions_elements_sizes;//the sizes of each dimension's elements
	uint8_t* data;
};

void idx_data_free(struct idx_data*);

struct idx_data* idx_load_from_idx_file(const char* file_name);

struct idx_data* idx_load_from_png_file(const char* file_name);

struct idx_data* idx_load_from_jpg_file(const char* file_name);

void idx_store_to_idx_file(const struct idx_data*, const char* file_name);

void idx_store_to_png_file(const struct idx_data*, const char* file_name);

void idx_store_to_jpg_file(const struct idx_data*, const char* file_name);

void* idx_get_element_ptr(const struct idx_data*, ...);

inline size_t idx_get_data_size(const struct idx_data* idx)
{
	return idx->dimensions_lengths[0] * idx->dimensions_elements_sizes[0];
}

inline size_t idx_get_elements_count(const struct idx_data* idx)
{
	return idx_get_data_size(idx) / idx_data_type_size(idx->data_type);
}