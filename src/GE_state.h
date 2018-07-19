#include <stdio.h>

#define COMPRESS       (1 << 0)  /* 0000000001 */
//#define WRITE          (1 << 1)  /* 0000000010 */
#define ERROR          (1 << 2)  /* 0000000100 */
#define PRINT          (1 << 3)  /* 0000001000 */
#define VERBOSE        (1 << 4)  /* 0000010000 */
#define LEX_MAP        (1 << 5)  /* 0000100000 */
#define LEX_DECOMPRESS (1 << 6)  /* 0001000000 */
#define LEX_IN         (1 << 7)  /* 0010000000 */
#define LEX_OUT        (1 << 8)  /* 0100000000 */
#define DECOMPRESS     (1 << 9)  /* 1000000000 */

#define state_init() 0
#define state_set(state, flag)	  state |= flag
#define state_unset(state, flag)  state &= ~flag
#define is_set(state, flag)       (state & flag) ? 1 : 0

