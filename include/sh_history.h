/* UniShell history is orginazed in Doubly Linked list */

#ifndef HISTORY_SHELL_H
#define HISTORY_SHELL_H

#include "command.h"


//
typedef struct HistoryEntry {
    struct HistoryEntry* next;
    struct HistoryEntry* prev;
    command_t* command;
} his_entry_t;

//
typedef struct History {
    his_entry_t* head;
    his_entry_t* tail;
    his_entry_t* curr_watching;
    unsigned size;
} history_t;


//
his_entry_t* shell_history_entry_new(command_t*);

//
history_t* shell_history_new();

//
void shell_history_append(history_t*, command_t*);

//
void shell_history_prepend(history_t*, command_t*);

//
void shell_history_show(history_t*);

//
void shell_history_free(history_t*);

//
char* shell_history_get_save_filename(const char*);


#endif // !HISTORY_SHELL_H