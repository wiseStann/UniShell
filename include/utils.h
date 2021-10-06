#ifndef SHELL_UTILS_H
#define SHELL_UTILS_H

#include "common.h"
#include "sh_history.h"

#define FILENAME_MAX_LEN FILENAME_MAX


// CURSOR MOVEMENTS MACROSES

extern void CURSOR_MOVE_LEFT(int);

extern void CURSOR_MOVE_RIGHT(int);


    // ARRAYS UTILS //

// checks if an array contains a given character
int char_array_contains(const char*, char);

// prepends a string to another string
void char_array_prepend(char*, const char*);

// appends a string to another string
void char_array_append(char*, const char*);

// trims a given string
void char_array_trim(char*);

// 
char* char_array_substring(const char*, unsigned, unsigned);

// checks if an array of commands contains a given command
int commands_array_contains(avaliable_cmd_entry_t*, const char*);

// gets the index of a given command in an array
int commands_array_get_index(avaliable_cmd_entry_t*, const char*);

//
void string_array_free(char**, unsigned int);


    // INPUT UTILS //

#ifdef __unix__

// gets a symbol in non-canonical mode of input
char getch();

#endif

// gets the key that was pressed 
int get_key_pressed();


    // FILE UTILS //

//
int file_save_history(history_t*);

//
int file_load_history(const char*);



#endif // !SHELL_UTILS_H