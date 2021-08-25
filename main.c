#include "include/common.h"
#include "include/input_buffer.h"
#include "include/command.h"
#include "include/parser.h"
#include "include/sh_history.h"



int main(int argc, char** argv)
{
    input_t* input = NULL;
    command_t* cmd = NULL;
    history_t* history = shell_history_new();
    his_entry_t* next_entry;
    int should_free_prompt_base = 0, cursor_pos_in_command = 0, non_canon_curr;
    unsigned previous_watched_command_len;
    char* esc_seq = (char*)sh_malloc(ESC_SEQUENCE_MAX_LEN);

    set_prompt_basename(DEFAULT_PROMPT_BASENAME);

    while (TRUE)
    {
        input = input_new();
        write_prompt_basename();

        /*  If arrow up or arrow down were pressed, then some manipulations with
         * shell history should be made and the history info should be shown to a user.
         */
        while ((non_canon_curr = get_key_pressed()) != ENTER_CODE)
        {
            switch (non_canon_curr)
            {
                case ARROW_UP:
                    if (history->curr_watching->next) {
                        previous_watched_command_len = input->size;
                        printf("\nCommand length: %d\n", previous_watched_command_len);
                        
                        history->curr_watching = history->curr_watching->next;
                        char tmp[history->curr_watching->command->length + ESC_SEQUENCE_MAX_LEN];
                        strcpy(tmp, history->curr_watching->command->content);

                        if (previous_watched_command_len > 0) {
                            sprintf(esc_seq, "\033[%dD", previous_watched_command_len);
                            char_array_prepend(tmp, esc_seq);
                        }
                        write(1, tmp, history->curr_watching->command->length + strlen(esc_seq));
                        input_buffer_pop_last_n(input, previous_watched_command_len);
                        printf("\nPUSHING TO BUFFER STRING: %s\n", history->curr_watching->command->content);
                        input_buffer_push_string(input, history->curr_watching->command->content, history->curr_watching->command->length);
                        printf("\nBUFFER NOW %s\n", input->buffer);

                        if (history->curr_watching->command->length < previous_watched_command_len) {
                            for (int i = history->curr_watching->command->length; i < previous_watched_command_len; i++) {
                                char space = SPACE_SYMBOL;
                                write(1, &space, 1);
                            }
                            sprintf(esc_seq, "\033[%dD", previous_watched_command_len - history->curr_watching->command->length);
                            write(1, esc_seq, strlen(esc_seq));
                        }
                    }
                    break;
                case ARROW_DOWN:
                    if (history->curr_watching->prev) {
                        previous_watched_command_len = input->size;
                        printf("\nCommand length: %d\n", previous_watched_command_len);

                        history->curr_watching = history->curr_watching->prev;
                        char tmp[history->curr_watching->command->length + ESC_SEQUENCE_MAX_LEN];
                        strcpy(tmp, history->curr_watching->command->content);

                        if (previous_watched_command_len > 0) {
                            sprintf(esc_seq, "\033[%dD", previous_watched_command_len);
                            char_array_prepend(tmp, esc_seq);
                        }
                        write(1, tmp, history->curr_watching->command->length + strlen(esc_seq));
                        input_buffer_pop_last_n(input, previous_watched_command_len);
                        printf("PUSHING TO BUFFER STRING: %s\n", history->curr_watching->command->content);
                        input_buffer_push_string(input, history->curr_watching->command->content, history->curr_watching->command->length);
                        printf("BUFFER NOW %s\n", input->buffer);

                        if (history->curr_watching->command->length < previous_watched_command_len) {
                            for (int i = history->curr_watching->command->length; i < previous_watched_command_len; i++) {
                                char space = SPACE_SYMBOL;
                                write(1, &space, 1);
                            }
                            sprintf(esc_seq, "\033[%dD", previous_watched_command_len - history->curr_watching->command->length);
                            write(1, esc_seq, strlen(esc_seq));
                        }
                    }
                    break;
                case BACKSPACE:
                    if (input->size > 0) {
                        write(1, "\033[1D \033[1D", strlen("\033[1D \033[1D"));
                        input_buffer_pop(input);
                    }
                    break;
                default:
                    write(1, &non_canon_curr, 1);
                    input_buffer_push(input, non_canon_curr);
                    cursor_pos_in_command++;
                    break;
            }
        }
        if (!input->size) { printf("\n"); continue; }

        putchar(non_canon_curr);
        input_buffer_push(input, '\0');

        // check if hte prompt basename has been changed. If so, we should free memory that
        // has been allocated for a new basename to avoid memory leaks
        if (strcmp(prompt_basename, DEFAULT_PROMPT_BASENAME)) should_free_prompt_base = 1;
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
    command_free(cmd);
    shell_history_free(history);

    if (should_free_prompt_base) free(prompt_basename);

    return 0;
}