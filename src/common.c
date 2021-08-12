#include "../include/common.h"


// available commands
avaliable_cmd_entry_t AVAILABLE_COMMANDS[COMMANDS_MAX_NUM] = {
    { .cmd_name = "echo", .description = "prints something to console or to the I/O streams" },
    { .cmd_name = "ls", .description = "shows all files and dirs in the current directory" },
    { .cmd_name = "pwd", .description = "shows path to the current directory" },
    { .cmd_name = "clear", .description = "clears the screen" },
    { .cmd_name = "exit", .description = "exits this applications" }, 
    { .cmd_name = "cmdslist", .description = "shows available commands" },
    { NULL, NULL },
};

const char SPEC_SYMBOLS[SPEC_SYMBOLS_MAX_NUM] = {
    '#', ' ', '\"', '\\', '/', '|', '>', '<', '\0',
};

/*
 *
 */
int
char_array_contains(const char* array, char symbol)
{
    for (int i = 0; array[i]; i++) {
        if (array[i] == symbol)
            return TRUE;
    }
    return FALSE;
}

/*
 *
 */
int
commands_array_contains(avaliable_cmd_entry_t* entries, const char* string)
{
    for (int i = 0; entries[i].cmd_name; i++) {
        if (!strcmp(entries[i].cmd_name, string)) {
            return TRUE;
        }
    }
    return FALSE;
}

/*
 *
 */
int
commands_array_get_index(avaliable_cmd_entry_t* entries, const char* string)
{
    for (int i = 0; entries[i].cmd_name; i++) {
        if (!strcmp(entries[i].cmd_name, string)) {
            return i;
        }
    }
    return -1;
}

/*
 *
 */
void commands_show()
{
    printf("Available commands:\n");
    for (int i = 0; AVAILABLE_COMMANDS[i].cmd_name; i++) {
        printf("-> Name: %s\n", AVAILABLE_COMMANDS[i].cmd_name);
        printf("   Description: %s\n", AVAILABLE_COMMANDS[i].description);
    }
}