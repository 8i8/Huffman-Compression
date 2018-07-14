#include "HC_state.h"

/*
 * state_init; Initialize the state struct.
 */
int state_init(void)
{
	return 0;
}

/*
 * state_set; Set flag by passing in appropriate bit field.
 */
int state_set(int state, unsigned token)
{
	return state |= token;
}

/*
 * state_unset: Unset flag by passing appropriate bitfiled.
 */
int state_unset(int state, unsigned token)
{
	return state &= ~token;
}
