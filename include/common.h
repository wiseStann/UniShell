#ifndef SHELL_COMMON_H
#define SHELL_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define TRUE 1
#define FALSE (!TRUE)

#define COMMANDS_MAX_NUM 16
#define SPEC_SYMBOLS_MAX_NUM 32

#define ENTER_CODE 10


extern const char* COMMANDS[COMMANDS_MAX_NUM];
// extern const unsigned int COMMANDS_NUM;

extern const char SPEC_SYMBOLS[SPEC_SYMBOLS_MAX_NUM];
extern const unsigned int SPEC_SYMBOLS_NUM;

//
int char_array_contains(const char*, unsigned int, char);


// specific symbols which can occur in a command 
enum SYMBOL_KIND {
    /* if the COMMENT_SYMBOL occurs, then all the following
     * symbols are insignificant and we can stop parsing a command 
     */ 
    COMMENT_SYMBOL = '#',

    /* if the SPACE_SYMBOL occurs, then we can stop parsing current
     * token and push it to the command arguments array
     */
    SPACE_SYMBOL = ' ',

    /* if the QUOTE_SYMBOL occurs, then the "sentence token" follows up
     * or is already successfully read. if the "sentence token" follows up,
     * we start reading it and looking forward for the second QUOTE_SYMBOL.
     * if the "sentence token" is already successfully read, we push it to
     * the command arguments array
     */
    DOUBLE_QUOTE_SYMBOL = '\"',

    /* 
     *
     */
    BACK_SLASH_SYMBOL = '\\',

    /*
     *
     */
    FORWARD_SLASH_SYMBOL = '/',

    /*
     *
     */
    TERMINAL_SYMBOL = '\0',
};


#endif // !SHELL_COMMON_H