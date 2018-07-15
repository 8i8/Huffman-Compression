#include <stdio.h>

#define COMPRESS     (1 << 0)  /* 00000001 */
#define WRITE        (1 << 1)  /* 00000010 */
#define ERROR        (1 << 2)  /* 00000100 */
#define PRINT        (1 << 3)  /* 00001000 */
#define VERBOSE      (1 << 4)  /* 00010000 */
#define LE_MAP       (1 << 5)  /* 00100000 */
#define LE_DECOMP    (1 << 6)  /* 01000000 */
#define LE_IN        (1 << 7)  /* 10000000 */
#define LE_OUT       (1 << 8)
#define DECOMPRESS   (1 << 9)  /* 00010000 */

#define state_init() 0
#define state_set(state, flag)	 state |= flag
#define state_unset(state, flag) state &= ~flag
#define is_set(state, flag)      state & flag

