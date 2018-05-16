#include <stdarg.h>

/* HC_Message_set: Compile a message and store it for handling */
void HC_Message_set(char *string, ...);
void HC_Message_append(char *string, ...);
void HC_Message_insert(char *string, ...);

/* HC_Message_print: Print message string and reset to empty string. */
char *HC_Message_get(void);

/* HC_Message_print: Write message to terminal and reset */
void HC_Message_print(void);

/* HC_Message_state: Returns the status of the message service, 0 as all clear, 1
 * if an message is pending. */
int HC_Message_state(void);

/* HC_Message_reset: Set to empty, with no message  */
void HC_Message_reset(void);

