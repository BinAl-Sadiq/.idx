#pragma once

#include <stdint.h>

enum idx_data_type
{
	IDX_UINT8 = 0x08,
	IDX_SINT8 = 0x09,
	IDX_SINT16 = 0x0B,
	IDX_SINT32 = 0x0C,
	IDX_FLOAT = 0x0D,
	IDX_DOUBLE = 0x0E
};

size_t idx_data_type_size(enum idx_data_type);