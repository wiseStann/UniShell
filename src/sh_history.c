#include "../include/sh_history.h"


/*
 *
 */
his_entry_t* shell_history_entry_new(command_t* command)
{
    his_entry_t* entry = (his_entry_t*)sh_malloc(sizeof(his_entry_t));
    entry->next = NULL;
    entry->prev = NULL;
    entry->command = command;

    return entry;
}

/*
 *
 */
history_t*
shell_history_new()
{
    history_t* history = (history_t*)sh_malloc(sizeof(history_t));
    history->head = NULL;
    history->curr_watching = shell_history_entry_new(NULL);
    history->curr_watching->next = history->head;
    history->size = 0;

    return history;
}

/*
 *
 */
void
shell_history_prepend(history_t* history, command_t* command)
{
    his_entry_t* entry = shell_history_entry_new(command);
    if (!history->head)
        history->head = entry;
    else {
        entry->next = history->head;
        history->head->prev = entry;
        history->head = entry;
    }
    history->curr_watching->next = history->head;
    history->size++;
}

/*
 *
 */
void
shell_history_show(history_t* his)
{
    his_entry_t* curr = his->head;
    printf("Current history:\n");
    while (curr) {
        printf("> %s;\n", curr->command->content);
        curr = curr->next;
    }
    printf("\n");
}

/*
 *
 */
void
shell_history_free(history_t* history)
{
    his_entry_t* curr = history->head;
    his_entry_t* tmp;
    while (curr) {
        tmp = curr->next;
        free(curr);
        curr = tmp;
    }
    free(history);
}
