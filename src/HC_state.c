#include "HC_state.h"

/*
 * state_init; Initialize the state struct.
 */
unsigned state_init(void)
{
	return 0;
}

/*
 * state_set; Set flag by passing in appropriate bit field.
 */
unsigned state_set(unsigned state, unsigned flag)
{
	return state |= flag;
}

/*
 * state_unset: Unset flag by passing appropriate bitfiled.
 */
unsigned state_unset(unsigned state, unsigned flag)
{
	return state &= ~flag;
}
