/* HC_Error_set: Erase any existing message and set a new message in its
 * place */
void HC_Error_set(char *input, ...);

/* HC_Error_append: Append to the current message */
void HC_Error_append(char *input, ...);

/* HC_Error_insert: Insert at the beginning of the current message */
void HC_Error_insert(char *input, ...);

/* HC_Error_get: Return message and reset to empty  */
char *HC_Error_get(void);

/* HC_Error_print: Write message to terminal and reset */
void HC_Error_print(void);

/* HC_Error_state: Returns the status of the message service, 0 as all clear, 1
 * if an message is pending */
int HC_Error_state(void);

/* HC_Error_reset: Set to empty, with no message  */
void HC_Error_reset(void);

