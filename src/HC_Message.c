#include "HC_Message.h"
#include "hc_msg.h"

static char message[MAX_LENGTH + 1];
static char store[MAX_LENGTH + 1];
static char send[MAX_LENGTH + 1];
static int state = 0;

/*
 * HC_Message_set: Erase any existing message and set a new message in its
 * place.
 */
void HC_Message_set(char *input, ...)
{
	store[0] = '\0';
	va_list va;
	va_start(va, input);
	if(_ds_message_set(message, input, &va))
		state = 1;
	va_end(va);

	_ds_write_to_string(0, store, message);
}

/*
 * HC_Message_append: Append to the current message.
 */
void HC_Message_append(char *input, ...)
{
	va_list va;
	va_start(va, input);
	_ds_message_set(message, input, &va);
	va_end(va);

	_ds_write_to_string(0, store, " ");
	_ds_write_to_string(0, store, message);
	state = 1;
}

/*
 * HC_Message_insert: Insert at the beginning of the current message.
 */
void HC_Message_insert(char *input, ...)
{
	va_list va;
	va_start(va, input);
	_ds_message_set(message, input, &va);
	va_end(va);

	_ds_write_to_string(1, store, " ");
	_ds_write_to_string(1, store, message);
	state = 1;
}

/*
 * HC_Message_get: Return message and reset to empty.
 */
char *HC_Message_get(void)
{
	strcpy(send, store);
	store[0] = '\0';
	state = 0;

	return send;
}

/*
 * HC_Message_print: Write message to terminal and reset.
 */
void HC_Message_print(void)
{
	_ds_write_to_string(1, store, "info: ");
	strcpy(send, store);
	store[0] = '\0';
	state = 0;
#ifdef _unix_
	write(1, send, strlen(send));
#endif
#ifndef _unix_
	printf("%s", send);
#endif
}

/*
 * HC_Message_state: Returns the status of the message service, 0 as all clear, 1
 * if an message is pending.
 */
int HC_Message_state(void)
{
	return state;
}

/*
 * HC_Message_reset: Set to empty, no message.
 */
void HC_Message_reset(void)
{
	store[0] = '\0';
	state = 0;
}
