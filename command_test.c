/**
 * Write tests for your command parsing library here.
 * 
 * If you wish to call helper functions from command.c besides
 * command_parse, command_free, and command_print, you must add their
 * headers to command.h first.
 */
#include <stdlib.h>
#include <stdio.h>
#include "command.h"

/*
 * Test command arrays.
 *
 * These are hard-coded command arrays with which command_print and
 * command_show can be tested. Those functions can be tested using
 * these command arrays without needing you to implement command_parse.
 *
 * Adding to these tests requires (1) adding new static array lines,
 * (2) adding them to the COMMAND_ARRAYS array, and (3) updating the
 * NUM_COMMAND_ARRAYS constant.
 */
static char* NANO_CMD[3] = { "nano", "foo.c", NULL };
static char* GCC_CMD[6] = { "gcc", "-Wall", "-o", "foo", "foo.c", NULL };


#define NUM_COMMAND_ARRAYS 2
static char** COMMAND_ARRAYS[NUM_COMMAND_ARRAYS] = {
  NANO_CMD,
  GCC_CMD
};

/*
 * Test command lines.
 *
 * These are hard-coded strings that will be parsed into command arrays
 * via command_parse.
 *
 * Adding to these tests requires (1) adding new lines to COMMAND_LINES,
 * and (2) updating the NUM_COMMAND_LINES constant.
 */
#define NUM_COMMAND_LINES 16
static char* COMMAND_LINES[NUM_COMMAND_LINES] = {
  "Hello world!  Please parse    me.",
  "A B&",
  " & this is invalid ",
  " I am &also invalid ",
  " This    has    a    lot of   spaces&     ",
  "ls -l   facs-labs       ",
  "       ls -l fcs-labs",
  " ls        -l   fcs-labs    ",
  "nano &",
  "nano&",
  "   nano&  ",
  "&uhoh ",
  "  &  uh oh",
  "uh & oh",
  "uh oh & &",
  "space space     &   ",
};


/**
 * Tests the behavior of all library functions on the given command line.
 */
void test_all(char* command) {
  int foreground;
  char** c;
  printf("Parsing: \"%s\"\n", command);
  c = command_parse(command, &foreground);
  if (c) {
    printf("Parsed %s command:\n", (foreground ? "foreground" : "background"));
    command_show(c);
    printf("As command line: ");
    command_print(c);
    printf("\n");
    command_free(c);
    printf("Freed command array.\n");
  } else {
    printf("Invalid command.\n");
  }
}

/**
 * Entry function. Tests all the specified command arrays and command lines.
*/
int main(int argc, char** argv) {

  for (int i = 0; i < NUM_COMMAND_ARRAYS; i++) {
    printf("Command array %d:\n", i);
    command_show(COMMAND_ARRAYS[i]);
    printf("As command line: ");
    command_print(COMMAND_ARRAYS[i]);
    printf("\n");
  }
  
  for (int i = 0; i < NUM_COMMAND_LINES; i++) {
    test_all(COMMAND_LINES[i]);
  }
  
  // main should technically return an int result.
  // 0 means success; nonzero means error.
  return 0;
}

