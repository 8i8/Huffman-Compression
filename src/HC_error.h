/* HC_error_set: Erase any existing message and set a new message in its
 * place */
void HC_error_set(char *input, ...);

/* HC_error_append: Append to the current message */
void HC_error_append(char *input, ...);

/* HC_error_insert: Insert at the beginning of the current message */
void HC_error_insert(char *input, ...);

/* HC_error_get: Return message and reset to empty  */
char *HC_error_get(void);

/* HC_error_print: Write message to terminal and reset */
void HC_error_print(void);

/* HC_error_state: Returns the status of the message service, 0 as all clear, 1
 * if an message is pending */
int HC_error_state(void);

/* HC_error_reset: Set to empty, with no message  */
void HC_error_reset(void);

