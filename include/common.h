#ifndef SHELL_COMMON_H
#define SHELL_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define TRUE 1
#define FALSE (!TRUE)

#define STATUS_SUCCESS TRUE
#define STATUS_FAILURE FALSE

#define COMMANDS_MAX_NUM 16
#define SPEC_SYMBOLS_MAX_NUM 32

#define ENTER_CODE 10


typedef struct avaliable_commands_entry {
    char* cmd_name;
    const char* description;
} avaliable_cmd_entry_t;

extern avaliable_cmd_entry_t AVAILABLE_COMMANDS[COMMANDS_MAX_NUM];

// commands aliases
typedef enum cmds_aliases {
    ECHO_CMD,
    LS_CMD,
    PWD_CMD,
    CLEAR_CMD,
    EXIT_CMD,
    CMDS_LIST_CMD,
} COMMANDS_ALIASES;

extern const char SPEC_SYMBOLS[SPEC_SYMBOLS_MAX_NUM];
extern const unsigned int SPEC_SYMBOLS_NUM;

//
int char_array_contains(const char*, char);

//
int commands_array_contains(avaliable_cmd_entry_t*, const char*);

//
int commands_array_get_index(avaliable_cmd_entry_t*, const char*);


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
    VERTICAL_BAR_SYMBOL = '|',

    /*
     *
     */
    LESS_THAN_SYMBOL = '<',

    /*
     *
     */
    GREATER_THAN_SYMBOL = '>',

    /*
     *
     */
    TERMINAL_SYMBOL = '\0',
};


//
void commands_show();


#endif // !SHELL_COMMON_H