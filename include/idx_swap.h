#pragma once

#include <stdint.h>

inline void* idx_swap16(const void* ptr)
{
	return (uint8_t[2]) { ((uint8_t*)ptr)[1], ((uint8_t*)ptr)[0] };
}

inline void* idx_swap32(const void* ptr)
{
	return (uint8_t[4]) { ((uint8_t*)ptr)[3], ((uint8_t*)ptr)[2], ((uint8_t*)ptr)[1], ((uint8_t*)ptr)[0] };
}

inline void* idx_swap64(const void* ptr)
{
	return (uint8_t[8]) { ((uint8_t*)ptr)[7], ((uint8_t*)ptr)[6], ((uint8_t*)ptr)[5], ((uint8_t*)ptr)[4], ((uint8_t*)ptr)[3], ((uint8_t*)ptr)[2], ((uint8_t*)ptr)[1], ((uint8_t*)ptr)[0] };
}

void* idx_swap_array(const void*ptr, size_t elements_count, enum idx_data_type data_type);
