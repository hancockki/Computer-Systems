/**
 * A simple command parsing library.
 *
 * This is a C "header file". It contains only the definitions
 * of functions, not their implementations. The actual
 * implementations are provided in the corresponding .c source file.
 */

#ifndef _COMMAND_H_
#define _COMMAND_H_

/** Parse a command-line string to a command array. */
char** command_parse(char* line, int* foreground);

/** Show the structure of a command array. */
void command_show(char** command);

/** Print a command array in the form of a command line. */
void command_print(char** command);

/** Free all parts of a command array. */
void command_free(char** command);

#endif // _COMMAND_H_
