#include <assert.h>

#define FAIL(m) (printf(__FILE__ ":%s():%d: error, %s.\n", __func__, __LINE__, m), err++)
#define WARNING(m) (printf(__FILE__ ":%s():%d: warning, %s.\n", __func__, __LINE__, m))

extern int err;

