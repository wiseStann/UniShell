/* UniShell history is orginazed in Doubly Linked list */

#ifndef HISTORY_SHELL_H
#define HISTORY_SHELL_H

#include "common.h"


typedef struct History {
    struct History* next;
    struct History* prev;
    command_t* command;
} history_t;


//
history_t* shell_history_new();

//
void shell_history_prepend(history_t*, command_t*);


#endif // !HISTORY_SHELL_H