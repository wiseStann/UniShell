#include "../include/common.h"


// available commands
const char* COMMANDS[COMMANDS_MAX_NUM] = {
    "echo", // prints something into console or to the I/O streams
    "ls", // shows all files and dirs in the current directory
    "pwd", // shows path to the current directory
    "clear", // clears the screen
    "exit", // exits this applications
};
// const unsigned int COMMANDS_NUM = sizeof(COMMANDS) / sizeof(*COMMANDS);

const char SPEC_SYMBOLS[SPEC_SYMBOLS_MAX_NUM] = {
    '#', ' ', '\"', '\\', '/',
};
const unsigned int SPEC_SYMBOLS_NUM = sizeof(SPEC_SYMBOLS) / sizeof(*SPEC_SYMBOLS);

//
int
char_array_contains(const char* array, unsigned int size, char symbol)
{
    for (int i = 0; i < size; i++) {
        if (array[i] == symbol)
            return TRUE;
    }
    return FALSE;
}