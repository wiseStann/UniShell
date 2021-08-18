#include "../include/sh_history.h"


/*
 *
 */
history_t*
shell_history_new()
{
    history_t* history = (history_t*)sh_malloc(sizeof(history_t));
    history->next = NULL;
    history->prev = NULL;
    history->command = (command_t*)sh_malloc(sizeof(command_t));

    return history;
}

/*
 *
 */
void
shell_history_prepend(history_t* his, command_t* command)
{
    if (!his)
        his = shell_history_new();
    his->command = command;

}