#include "idx_error.h"

enum idx_error_type idx_error = IDX_NO_ERROR;

enum idx_error_type idx_get_error(void)
{
	enum idx_error_type err = idx_error;
	idx_error = IDX_NO_ERROR;
	return err;
}
