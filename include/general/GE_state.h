#ifndef GE_STATE
#define GE_STATE

#define DTA_EOF          (1 <<  0)
#define DECOMPRESS       (1 <<  1)
#define COMPRESS         (1 <<  2)
#define VERBOSE          (1 <<  3)
#define FORCE            (1 <<  4)
#define MONO             (1 <<  5)
#define MULTI            (1 <<  6)
#define PRINT            (1 <<  7)
#define ESC              (1 <<  8)
#define ERROR            (1 <<  9)
#define LEX_MAP          (1 << 10)
#define LEX_DECOMPRESS   (1 << 11)
#define LEX_FILENAME     (1 << 12)
#define LEX_CHAR         (1 << 13)
#define LEX_ERROR        (1 << 14)
#define LEX_EOF          (1 << 15)

#define state_init()              0
#define state_set(state, flag)	  ((state) |= (flag))
#define state_unset(state, flag)  ((state) &= ~(flag))
#define is_set(state, flag)       ((state) & (flag))

/*
 * is_set: Returns the current state of a flag.
 */
//int is_set(int state, int flag);

#endif
