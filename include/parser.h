#ifndef SHELL_PARSER_H
#define SHELL_PARSER_H

#include "command.h"


// parses a given command and returns command_t*
command_t* command_parse_new(char*, int);

// handle a given command
int command_handle(command_t*);

// parses a given command and validates it
int command_parse_is_valid(command_t*);

// // checks if the command basename exists in available commands list
// int command_parse_basename_exists(command_t*);

// checks if the command syntax is valid
int command_parse_syntax_is_valid(command_t*);

// parses a given command and returns the number of its arguments
unsigned int command_parse_arguments_number(command_t*);

// gets the basename of a given command, i.e. the name of the command
char* command_parse_get_basename(command_t*);


#endif // ! SHELL_PARSER_H