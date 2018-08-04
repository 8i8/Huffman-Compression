
#define DECOMPRESS     (1 <<  1)
#define COMPRESS       (1 <<  2)
#define VERBOSE        (1 <<  3)
#define FORCE          (1 <<  4)
#define MONO           (1 <<  5)
#define MULTI          (1 <<  6)
#define PRINT          (1 <<  7)
#define ESC            (1 <<  8)
#define ERROR          (1 <<  9)

#ifndef _state_
#define _state_
#define state_init()              0
#define state_set(state, flag)	  (state |= flag)
#define state_unset(state, flag)  (state &= ~flag)
#define is_set(state, flag)       ((state & flag) ? 1 : 0)
#endif

