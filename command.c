/**
 * Lab 2 - Programming Pointers
 *
 * Sam Harder, Kim Hancock
 *
 * command.c - Source file with your parsing library implementation.
 *
 */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "command.h"

/**
 * command_parse:
 *
 * Parse a command-line string, report foreground/background status,
 * and return a command array.  Each element in the command array
 * corresponds to one word in the command string, in order.
 *
 * A command with '&' after the last word is a background command.
 * The '&' is not part of the resulting command array.  Repetitions or
 * other placements of '&' constitute invalid command lines.
 *
 * Parameters:
 *
 * - line: non-NULL pointer to a command-line string.
 *
 * - foreground: non-NULL pointer to an int where foreground (1)/
 *   background (0) status of the command line should be stored.
 * 
 * Return:
 *
 * - a pointer to a NULL-terminated array of strings (char**) if given
 *   a valid command-line string.
 *
 * - NULL if given a command-line string with invalid placement of '&'
 */
char** command_parse(char* line, int* foreground) {
  // Check arguments: must be non-NULL pointers.
  assert(line);
  assert(foreground);
  
  int word_count = 0; // counts num of words in line 
  int space = 1; // tracks whether prev. char was a space
  // We set foreground to 1 until we observe an ampersand.
  *foreground = 1;

  // Iterate through p, count words, and detect invalid ampersand placement.
  for(char* p = line; *p; p++){
    
    // Invalid Ampersand LOGIC
    if(!*foreground && *p != ' '){
      return NULL; // We saw a letter after an ampersand
    }
    if(*p == '&'){
      *foreground = 0; // ampersand detected
    }

    // Word Count LOGIC
    // If we see a new word, we increment word_count
    if(*p != ' ' && *p != '&' && space){ 
      space = 0;
      word_count += 1;
    } 
    if(*p == ' '){
      space = 1; // set to 1 so we can detect new words
    }
  }

  // Allocates memory for command array
  char** command_array = malloc((word_count + 1)*sizeof(char*));
  
  char last_char = ' '; // tracks last character seen
  // word_pointer tells us where to put pointer for each word
  char** word_pointer = command_array;
  int letter_count = 0; // counts letters in each word

  // Allocates memory for each word in command_array
  for(char* p = line; *p; p++){
    // If we're within a word, i.e. if *p is not '&' or ' '
    if(!(*p == ' ' ||  *p == '&')){
      letter_count++; 
      if(!*(p + 1)){ // If at end of string, we allocate for current word
        *word_pointer = (char*)malloc(sizeof(char)*(letter_count + 1));
	word_pointer++;
      }
    }
    else{
      // If *p is '&' or ' ' we check to see if last_char was a letter. 
      // If it was we just finished a word and allocate space. 
      if(last_char != ' ' && last_char != '&'){
        *word_pointer = (char*)malloc(sizeof(char)*(letter_count + 1));
	word_pointer++;
      } 
      letter_count = 0; //prepares letter_count for next word
    }
    last_char = *p; 
  }

  word_pointer = command_array; // set word_pointer to start of command_array
  last_char = ' '; 
  char* letter_pointer = *word_pointer; // points to each letter in a word

  // Finally, we fill in memory allocated for each word.
  for(char* p = line; *p; p++) {
    // If we're in a word:
    if(!(*p == ' ' || *p == '&')){ 
      *letter_pointer = *p; // set *letter_pointer to current char
      letter_pointer++; // increment letter_pointer for next letter

      if(!*(p + 1)){ // If we are at the end of the string, then our word is finished.
        *letter_pointer = 0; // Add null character to finish string.
        word_pointer++; // prepare to add NULL to command_array
      }
    }
    // If we're not in a word
    else{
      // Check to see if we just finished a word
      if(last_char != ' ' && last_char != '&'){
        *letter_pointer = 0; // Add null character to finish string
	word_pointer++; // prepare word_pointer for next word.
	letter_pointer = *word_pointer; // point letter_pointer to start of next word.
      }
    }
    last_char = *p;
  } 
  *word_pointer = NULL; // Append NULL to end of Command Array
  return command_array;
}

/**
 * command_show:
 *
 * Print the structure of a command array.
 *
 * Parameters:
 *
 * - command: a non-NULL pointer to the command array to print.
 */
void command_show(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command);

  // Iterate through command, printing each word surrounded
  // by quotation marks to make any padded spaces visible.
  for(char** p = command; *p != NULL; p++){
    printf("\"%s\" ",*p); 
  }
  printf("\n");
}

/**
 * command_print:
 *
 * Print a command array in the form of a command-line string.
 * You should *not* include a newline at the end.
 *
 * Parameters:
 *
 * - command: non-NULL pointer to the command array to print.
 */
void command_print(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command);

  for(char** p = command; *p ; p++){
    if(!*(p+1)){ 
      // If we are at the last word don't append a space
      printf("%s", *p); 
    }
    else{ 
      // Not at last word, so print with space appended.
      printf("%s ", *p);
    }
  }
}

/**
 * command_free:
 *
 * Free all parts of a command array created by command_parse.
 *
 * Parameters:
 *
 * - command: non-NULL pointer to the command array to free.
 */
void command_free(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command);

  //Walk through command array and free each pointer
  for(char** p = command; *p; p++){
    free(*p);
  }
  free(command);
}

