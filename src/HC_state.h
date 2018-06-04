
#define READ         (1 << 0)  /* 00000001 */
#define WRITE        (1 << 1)  /* 00000010 */
#define ERROR        (1 << 2)  /* 00000100 */
#define PRINT        (1 << 3)  /* 00001000 */
#define DECOMP       (1 << 4)  /* 00010000 */
#define KEY_5        (1 << 5)  /* 00100000 */
#define KEY_6        (1 << 6)  /* 01000000 */
#define KEY_7        (1 << 7)  /* 10000000 */

/* init_state; Initialize the state struct */
int state_init(void);

/* state; Set flag by passing in appropriate bit field */
int state_set(unsigned s);
