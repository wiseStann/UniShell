#ifndef SHELL_COMMON_H
#define SHELL_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

#include <sys/wait.h>
#include <sys/unistd.h>

#include "sh_history.h"


#define TRUE 1
#define FALSE 0

#define STATUS_SUCCESS TRUE
#define STATUS_FAILURE -TRUE

#define COMMANDS_MAX_NUM 16
#define SPEC_SYMBOLS_MAX_NUM 32

#define ESC_SEQUENCE_MAX_LEN 16

#define DEFAULT_PROMPT_BASENAME "UniShell$>"
#define DEFAULT_HIST_FILENAME "uni_shell_hist"
#define HIST_FILE_EXT ".hist"

#define STRING(_x) #_x

#define SESSION_ID_MAX_LEN 16

extern char* prompt_basename;
extern history_t* history;
extern char* hist_file_name;
extern char* session_id;


/* shows a given error message to the user
   it's like a simple printf */
void throw_error(const char* fmt, ...);


// available command structure
typedef struct avaliable_commands_entry {
    char* cmd_name;
    const char* description;
} avaliable_cmd_entry_t;

extern avaliable_cmd_entry_t AVAILABLE_COMMANDS[COMMANDS_MAX_NUM];

extern const char SPEC_SYMBOLS[SPEC_SYMBOLS_MAX_NUM];
extern const unsigned int SPEC_SYMBOLS_NUM;


///////////////////////////////////////////////////////////////////////////////////


// commands aliases
typedef enum cmds_aliases {
    ECHO_CMD,
    LS_CMD,
    CAT_CMD,
    PWD_CMD,
    CLEAR_CMD,
    EXIT_CMD,
    CHANGE_PB_CMD,
    CMDS_LIST_CMD,

    // history commands
    HIST_SHOW_CMD,
    HIST_SAVE_CMD,
    HIST_LOAD_CMD,
    HIST_FREE_CMD,
} COMMANDS_ALIASES;


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

    /*
     *
     */
    DOLLAR_SYMBOL = '$',


    /*  If TERMINAL_SYMBOL occurs, then the end of the string (C string) is reached. 
     */
    TERMINAL_SYMBOL = '\0',
};


// some keys ascii codes for keyboard usage detection
// these keys can either be independent or be part of another keys combination
typedef enum keys_codes {
    BS_KEY = 8,
    BS_KEY2 = 127,
    ENTER_CODE = 10,
    ESCAPE_CODE = 27,
    A_KEY = 65,
    B_KEY = 66,
    C_KEY = 67,
    D_KEY = 68,
} KEYS_CODES;


// special keys codes
// some of these keys actually don't have ascii code, but some are just aliases for ascii entities
typedef enum spec_keys_codes {
    ARROW_UP = 128,
    ARROW_DOWN = 129,
    ARROW_RIGHT = 130,
    ARROW_LEFT = 131,
    BACKSPACE = 132,
    ANOTHER_KEY = 133,
} SPEC_KEYS_CODES;


//////////////////////////////////////////////////////////////////////////////////////////


// custom allocation function
void* sh_malloc(unsigned int);

//
void set_session_id();

// changes the prompt basename
void set_prompt_basename(const char*);

//
void write_prompt_basename();

//
void set_history_filename(const char*);

// shows available commands
void commands_show();



#endif // !SHELL_COMMON_H