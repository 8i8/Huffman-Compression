
#define LEX_META       (1 << 2)
#define LEX_MAP        (1 << 2)
#define LEX_DECOMP     (1 << 3)
#define LEX_OUT        (1 << 4)

#ifndef _state_
#define _state_
#define state_init()              0
#define state_set(state, flag)	  (state |= flag)
#define state_unset(state, flag)  (state &= ~flag)
#define is_set(state, flag)       ((state & flag) ? 1 : 0)
#endif
