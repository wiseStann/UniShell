#ifndef SHELL_UTILS_H
#define SHELL_UTILS_H

#include "common.h"
#include "sh_history.h"


    // ARRAYS UTILS //

// checks if an array contains a given character
int char_array_contains(const char*, char);

// prepends a string to another string
void char_array_prepend(char*, const char*);

// checks if an array of commands contains a given command
int commands_array_contains(avaliable_cmd_entry_t*, const char*);

// gets the index of a given command in an array
int commands_array_get_index(avaliable_cmd_entry_t*, const char*);

//
void string_array_free(char**, unsigned int);


    // INPUT UTILS //

// gets a symbol in non-canonical mode of input
extern char getch();

// gets the key that was pressed 
extern int get_key_pressed();


    // FILE UTILS //

extern void file_write_history(const char*, history_t*);



#endif // !SHELL_UTILS_H