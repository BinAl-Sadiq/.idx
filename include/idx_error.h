#pragma once

enum idx_error_type
{
	IDX_NO_ERROR,
	IDX_INVALID_FIRST_TWO_BYTES,
	IDX_INVALID_DATA_TYPE,
	IDX_NO_DIMENSIONS,
	IDX_COULD_NOT_OPEN_FILE,
	IDX_MEMORY_SHORTAGE,
	IDX_COULD_NOT_LOAD_IMAGE
};

enum idx_error_type idx_get_error(void);
