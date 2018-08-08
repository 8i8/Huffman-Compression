#include <assert.h>

#define FAIL(m) (printf(__FILE__ ":%s():%d: error, %s.\n", __func__, __LINE__, m), err++), \
	assert(err == 0)

extern int err;

