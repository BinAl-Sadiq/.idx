#include "idx_data_type.h"

size_t idx_data_type_size(enum idx_data_type data_type)
{
	switch (data_type)
	{
	case IDX_UINT8:
	case IDX_SINT8:
		return 1;
	case IDX_SINT16:
		return 2;
	case IDX_SINT32:
		return 4;
	case IDX_FLOAT:
		return sizeof(float);
	case IDX_DOUBLE:
		return sizeof(double);
	default://invalid data type
		return 0;
	}
}