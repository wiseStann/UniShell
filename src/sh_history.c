#include "../include/common.h"
#include "../include/utils.h"
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
    history->tail = NULL;
    history->curr_watching = NULL;
    history->size = 0;

    return history;
}

/*
 *
 */
void
shell_history_append(history_t* history, command_t* command)
{
    his_entry_t* entry = shell_history_entry_new(command);

    if (!history)
        history = shell_history_new();
    if (!history->head)
        history->head = entry;
    else {
        his_entry_t* curr = history->head;
        while (curr->next) {
            curr = curr->next;
        }
        entry->prev = curr;
        curr->next = entry;
        history->tail = curr->next;
    }
    history->size++;
}

/*
 *
 */
void
shell_history_prepend(history_t* history, command_t* command)
{
    his_entry_t* entry = shell_history_entry_new(command);

    if (!history)
        history = shell_history_new();
    if (!history->head) {
        history->head = entry;
        history->tail = history->head;
    } else {
        entry->next = history->head;
        history->head->prev = entry;
        history->head = entry;
    }
    history->curr_watching = history->head->prev;
    history->size++;
}

/*
 *
 */
void
shell_history_show(history_t* his)
{
    his_entry_t* curr = his->tail;
    while (curr) {
        printf("> %s;\n", curr->command->content);
        curr = curr->prev;
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

/*
 *
 */
char*
shell_history_get_save_filename(const char* id)
{
    char* his_fn = (char*)sh_malloc(FILENAME_MAX_LEN);
    strcpy(his_fn, hist_file_name);
    char_array_append(his_fn, id);
    char_array_append(his_fn, HIST_FILE_EXT);
    return his_fn;
}