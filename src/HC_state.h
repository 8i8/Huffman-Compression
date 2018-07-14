#include <stdio.h>

#define READ         (1 << 0)  /* 00000001 */
#define WRITE        (1 << 1)  /* 00000010 */
#define ERROR        (1 << 2)  /* 00000100 */
#define PRINT        (1 << 3)  /* 00001000 */
#define DECOMP       (1 << 4)  /* 00010000 */
#define LE_MAP       (1 << 5)  /* 00100000 */
#define LE_DECOMP    (1 << 6)  /* 01000000 */
#define LE_IN        (1 << 7)  /* 10000000 */
#define LE_OUT       (1 << 8)

/* init_state; Initialize the state struct */
unsigned state_init(void);

/* state; Set flag by passing in appropriate bit field */
unsigned state_set(unsigned state, unsigned flag);

/* state_unset: Unset flag by passing appropriate bitfiled */
unsigned state_unset(unsigned state, unsigned flag);
