
#define DECOMPRESS     (1 <<  1)
#define COMPRESS       (1 <<  2)
#define VERBOSE        (1 <<  3)
#define FORCE          (1 <<  4)
#define MONO           (1 <<  5)
#define PRINT          (1 <<  6)
#define ERROR          (1 <<  7)
#define LEX_META_DATA  (1 <<  8)
#define LEX_MAP        (1 <<  9)
#define LEX_DECOMPRESS (1 << 10)
#define LEX_OUT        (1 << 11)

#define state_init()              0
#define state_set(state, flag)	  (state |= flag)
#define state_unset(state, flag)  (state &= ~flag)
#define is_set(state, flag)       ((state & flag) ? 1 : 0)

