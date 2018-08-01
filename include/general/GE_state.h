
#define COMPRESS       (1 <<  1)  /* 00000000010 */
#define MONO           (1 <<  2)  /* 00000000100 */
#define ERROR          (1 <<  3)  /* 00000001000 */
#define PRINT          (1 <<  4)  /* 00000010000 */
#define VERBOSE        (1 <<  5)  /* 00000100000 */
#define LEX_MAP        (1 <<  6)  /* 00001000000 */
#define LEX_DECOMPRESS (1 <<  7)  /* 00010000000 */
#define LEX_IN         (1 <<  8)  /* 00100000000 */
#define LEX_OUT        (1 <<  9)  /* 01000000000 */
#define DECOMPRESS     (1 << 10)  /* 10000000000 */

#define state_init()              0
#define state_set(state, flag)	  (state |= flag)
#define state_unset(state, flag)  (state &= ~flag)
#define is_set(state, flag)       ((state & flag) ? 1 : 0)

