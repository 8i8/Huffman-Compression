
#define DECOMPRESS       (1 <<   0)
#define COMPRESS         (1 <<   1)
#define VERBOSE          (1 <<   2)
#define FORCE            (1 <<   3)
#define MONO             (1 <<   4)
#define MULTI            (1 <<   5)
#define PRINT            (1 <<   6)
#define ESC              (1 <<   7)
#define ERROR            (1 <<   8)
#define LEX_MAP          (1 <<   9)
#define LEX_DECOMPRESS   (1 <<  10)
#define LEX_FILENAME     (1 <<  11)
#define LEX_CHAR         (1 <<  12)
#define LEX_ERROR        (1 <<  13)

#ifndef _state_
#define _state_
#define state_init()              0
#define state_set(state, flag)	  ((state) |= (flag))
#define state_unset(state, flag)  ((state) &= ~(flag))
#define is_set(state, flag)       ((state) & (flag))
#endif

/*
 * is_set: Returns the current state of a flag.
 */
//int is_set(int state, int flag);

