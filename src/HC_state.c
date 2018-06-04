#include "HC_state.h"

unsigned state;

/*
 * state_init; Initialize the state struct.
 */
int state_init(void)
{
	return state = 0;
}

/*
 * state_set; Set flag by passing in appropriate bit field.
 */
int state_set(unsigned s)
{
	return state |= s;
}

