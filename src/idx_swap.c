#include "idx_swap.h"

#include <malloc.h>
#include <memory.h>

#include "idx_data_type.h"
#include "idx_error.h"

extern enum idx_error_type idx_error;

void* idx_swap_array(const void* ptr, size_t elements_count, enum idx_data_type data_type)
{
	size_t data_type_size = idx_data_type_size(data_type);
	if (!data_type_size)
	{
		idx_error = IDX_INVALID_DATA_TYPE;
		return 0;
	}

	void* swapped_array = malloc(elements_count * data_type_size);
	if (!swapped_array)
	{
		idx_error = IDX_MEMORY_SHORTAGE;
		return 0;
	}

	switch (data_type)
	{
	case IDX_UINT8:
	case IDX_SINT8:
		memcpy(swapped_array, ptr, elements_count * data_type_size);
		break;
	case IDX_SINT16:
		for (size_t i = 0; i < elements_count; i++) ((int16_t*)swapped_array)[i] = *(int16_t*)idx_swap16((uint16_t*)ptr + i);
		break;
	case IDX_SINT32:
		for (size_t i = 0; i < elements_count; i++) ((int32_t*)swapped_array)[i] = *(int32_t*)idx_swap32((int32_t*)ptr + i);
		break;
	case IDX_FLOAT:
		for (size_t i = 0; i < elements_count; i++) ((float*)swapped_array)[i] = *(float*)idx_swap32((float*)ptr + i);
		break;
	case IDX_DOUBLE:
		for (size_t i = 0; i < elements_count; i++) ((double*)swapped_array)[i] = *(double*)idx_swap64((double*)ptr + i);
		break;
	}
	
	return swapped_array;
}
