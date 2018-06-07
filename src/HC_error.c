#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

#define MAX_LENGTH 4096

static char error[MAX_LENGTH + 1];
static char store[MAX_LENGTH + 1];
static char send[MAX_LENGTH + 1];
static int state = 0;

/*
 * _place_at_front: helper function, put the prologue string at the beginning of
 * the message.
 */
static void _place_at_front(char *store, char *message, int len_store, int len_message)
{
	char transfer[MAX_LENGTH];

	memcpy(transfer, store, len_store + 1);
	memcpy(store, message, len_message + 1);
	strcat(store, transfer);
}

/*
 * _write_to_string: Write the prologue string to the stored message.
 */
static char *_ds_write_to_string(int reverse, char *store, char *message)
{
	int len_store, len_message;

	len_store = strlen(store);
	len_message = strlen(message);

	/* String order, the 4 is for the ellipsis if the string does not fit
	 * and one for a space between the phrases. */
	if (MAX_LENGTH - len_store > len_message + 5) {
		if (reverse)
			_place_at_front(store, message, len_store, len_message);
		else
			strcat(store, message);
	} else
		strcat(store, " ...");

	return store;
}

/*
 * HC_Message_set: Compile a message and store it for further handling.
 */
static int _ds_message_set(char *message, char *prologue, va_list *va)
{
	char *head, *va_str;
	int va_num;
	double va_flo;
	unsigned int va_unum;
	size_t va_size_t;
	head = message;

	while (*prologue)
	{
		if (*prologue != '%') {
			*message++ = *prologue++;
			continue;
		}
		switch (*++prologue)
		{
			case 's':
				for (va_str = va_arg(*va, char *); *va_str; va_str++)
					*message++ = *va_str;
				break;
			case 'd':
				va_num = va_arg(*va, int);
				message += sprintf(message, "%d", va_num);
				break;
			case 'f':
				va_flo = va_arg(*va, double);
				message += sprintf(message, "%f", va_flo);
				break;
			case 'u':
				va_unum = va_arg(*va, unsigned int);
				message += sprintf(message, "%u", va_unum);
				break;
			case 'l':
				if (*(prologue+1) == 'u') {
					prologue++;
					va_size_t = va_arg(*va, size_t);
					message += sprintf(message, "%lu", va_size_t);
				} else if (!isspace(*(prologue+1)))
					prologue++;
				break;
			default:
				break;
		}
		prologue++;
	}
	*message = '\0';

	return message - head;
}

/*
 * HC_error_state: Returns the status of the message service, 0 as all clear, 1
 * if an message is pending.
 */
int HC_error_state(void)
{
	return state;
}

/*
 * HC_error_reset: Set to empty, with no message.
 */
void HC_error_reset(void)
{
	store[0] = '\0';
	state = 0;
}

/*
 * HC_error_set: Erase any existing message and set a new message in its
 * place.
 */
void HC_error_set(char *prologue, ...)
{
	store[0] = '\0';
	va_list va;
	va_start(va, prologue);
	if(_ds_message_set(error, prologue, &va))
		state = 1;
	va_end(va);

	_ds_write_to_string(0, store, error);
}

/*
 * HC_error_append: Append to the current message.
 */
void HC_error_append(char *prologue, ...)
{
	va_list va;
	va_start(va, prologue);
	_ds_message_set(error, prologue, &va);
	va_end(va);

	_ds_write_to_string(0, store, " ");
	_ds_write_to_string(0, store, error);
	state = 1;
}

/*
 * HC_error_insert: Insert at the beginning of the current message.
 */
void HC_error_insert(char *prologue, ...)
{
	va_list va;
	va_start(va, prologue);
	_ds_message_set(error, prologue, &va);
	va_end(va);

	_ds_write_to_string(1, store, " ");
	_ds_write_to_string(1, store, error);
	state = 1;
}

/*
 * HC_error_get: Return message and reset to empty.
 */
char *HC_error_get(void)
{
	strcpy(send, store);
	store[0] = '\0';
	state = 0;

	return send;
}

/*
 * HC_error_print: Write message to terminal and reset.
 */
void HC_error_print(void)
{
	_ds_write_to_string(1, store, "error: ");
	strcpy(send, store);
	store[0] = '\0';
	state = 0;
#ifdef _unix_
	write(stderr, send, strlen(send));
#endif
#ifndef _unix_
	fprintf(stderr, "%s", send);
#endif
}

