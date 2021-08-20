#include "include/common.h"
#include "include/parser.h"
#include "include/input_buffer.h"
#include "include/sh_history.h"


int main(int argc, char** argv)
{
    input_t* input = NULL;
    command_t* cmd = NULL;
    history_t* history = shell_history_new();
    his_entry_t* next_entry;
    int curr, non_canon_curr;

    set_prompt_basename(DEFAULT_PROMPT_BASENAME);

    while (TRUE) {
        input = input_new();

        /*  If arrow up or arrow down were pressed, then some manipulations with
         * shell history should be made and the history info should be shown to a user.
         */
        non_canon_curr = get_key_pressed();
        if (non_canon_curr > 127) {
            if (history->head) {
                switch (non_canon_curr)
                {
                    case ARROW_UP:
                        if (history->curr_watching->next) {
                            history->curr_watching = history->curr_watching->next;
                            write(0, history->curr_watching->command->content, history->curr_watching->command->length);
                            goto history_edit;
                        }
                        break;
                    case ARROW_DOWN:
                        if (history->curr_watching->prev) {
                            history->curr_watching = history->curr_watching->prev;
                            write(0, history->curr_watching->command->content, history->curr_watching->command->length);
                            goto history_edit;
                        }
                        break;
                }
            } else {
                putc('\n', stdout);
            }
        } else {
            putc(non_canon_curr, stdout);
            input_buffer_push(input, non_canon_curr);

history_edit:
            while ((curr = getchar()) != ENTER_CODE) {
                if (curr != BACK_SLASH_SYMBOL)
                    input_buffer_push(input, curr);
            }
            input_buffer_push(input, '\0');
            if (!strcmp(input->buffer, "exit")) break;

            cmd = command_parse_new(input->buffer);
            if (cmd) {
                int status = command_handle(cmd);
                if (status == -1)
                    printf("Invalid set of arguments was given to the command '%s'\n", cmd->name);
                shell_history_prepend(history, cmd);
                // shell_history_show(history);
            }

            input_free(input);
        }
    }
    command_free(cmd);
    shell_history_free(history);

    return 0;
}