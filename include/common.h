#ifndef SHELL_COMMON_H
#define SHELL_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/unistd.h>

#define TRUE 1
#define FALSE (!TRUE)

#define STATUS_SUCCESS TRUE
#define STATUS_FAILURE FALSE

#define COMMANDS_MAX_NUM 16
#define SPEC_SYMBOLS_MAX_NUM 32

#define ENTER_CODE 10


// available command structure
typedef struct avaliable_commands_entry {
    char* cmd_name;
    const char* description;
} avaliable_cmd_entry_t;

extern avaliable_cmd_entry_t AVAILABLE_COMMANDS[COMMANDS_MAX_NUM];

// commands aliases
typedef enum cmds_aliases {
    ECHO_CMD,
    LS_CMD,
    CAT_CMD,
    PWD_CMD,
    CLEAR_CMD,
    EXIT_CMD,
    CMDS_LIST_CMD,
} COMMANDS_ALIASES;

extern const char SPEC_SYMBOLS[SPEC_SYMBOLS_MAX_NUM];
extern const unsigned int SPEC_SYMBOLS_NUM;

// checks if an array contains a given character
int char_array_contains(const char*, char);

// checks if an array of commands contains a given command
int commands_array_contains(avaliable_cmd_entry_t*, const char*);

// gets the index of a given command in an array
int commands_array_get_index(avaliable_cmd_entry_t*, const char*);


// specific symbols which can occur in a command 
enum SYMBOL_KIND {
    /*  If COMMENT_SYMBOL occurs, then all the following
     * symbols are insignificant and we can stop parsing a command 
     */ 
    COMMENT_SYMBOL = '#',

    /* If SPACE_SYMBOL occurs, then we can stop parsing current
     * token and push it to the command arguments array
     */
    SPACE_SYMBOL = ' ',

    /*  If QUOTE_SYMBOL occurs, then the "sentence token" follows up
     * or is already successfully read. if the "sentence token" follows up,
     * we start reading it and looking forward for the second QUOTE_SYMBOL.
     * If the "sentence token" is already successfully read, we push it to
     * the command arguments array
     */
    DOUBLE_QUOTE_SYMBOL = '\"',

    /*  If BACK_SLASH_SYMBOL occurs and the newline character follows, then 
     * it is treated as a line continuation (that is, it is removed from the
     * input stream and effectively ignored). If there's no newline character
     * following the BACK_SLASH_SYMBOL, then the exeception must be raised.
     */
    BACK_SLASH_SYMBOL = '\\',

    /*  FORWARD_SLASH_SYMBOL actually serves as a path separator. But there can
     * be some exceptions.
     */
    FORWARD_SLASH_SYMBOL = '/',

    /*  If VERTICAL_BAR_SYMBOL occurs, then the "pipe" should be
     * created between two processes. The first one takes the command part
     * to VERTICAL_BAR_SYMBOL's left starting with the previous VERTICAL_BAR_SYMBOL,
     * if it exists, or with the first character of the command, if it doesn't.
     * The second one does in the mirror way, getting the commands to VERTICAL_BAR_SYMBOL's
     * right. And so on, organizing something that's called a pipeline.
     *  Example:
     * shell>$ ps aux | grep [k]de | gawk '{ print $2}' 
     */
    VERTICAL_BAR_SYMBOL = '|',

    /*  If LESS_THAN_SYMBOL occurs, then the data from a given source should be
     * pulled in a current stream.
     */
    LESS_THAN_SYMBOL = '<',

    /*  If a single GREATER_THAN_SYMBOL occurs, then the output streams
     * shoud be redirected to some input stream and rewrite its contents.
     * If a double GREATER_THAN_SYMBOL occurs, then the output streams
     * should be redirected to some input stream and write after its contents.
     */
    GREATER_THAN_SYMBOL = '>',

    /*
     *
     */
    HYPHEN_SYMBOL = '-',

    /*
     *
     */
    DOT_SYMBOL = '.',


    /*  If TERMINAL_SYMBOL occurs, then the end of the string (C string) is reached. 
     */
    TERMINAL_SYMBOL = '\0',
};


// custom allocation function
void* sh_malloc(unsigned int bytes);

// shows available commands
void commands_show();



#endif // !SHELL_COMMON_H