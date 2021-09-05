#include "include/common.h"
#include "include/input_buffer.h"
#include "include/command.h"
#include "include/parser.h"
#include "include/sh_history.h"
#include "include/utils.h"





/*
        SAVING SESSION HISTORY TO THE FILE
        LOADING HISTORY FROM THE FILE BY A GIVEN ID
*/








int main(int argc, char** argv)
{
    set_session_id();
    set_prompt_basename(DEFAULT_PROMPT_BASENAME);
    set_history_filename(DEFAULT_HIST_FILENAME);

    // session history initialization
    history = shell_history_new();

    input_t* input = NULL;
    command_t* cmd = NULL;
    his_entry_t* entry_to_retrieve = NULL;

    int free_prompt_base = 0, free_hist_filename = 0;
    int cursor_pos_in_command = 0, non_canon_curr;
    unsigned previous_watched_command_len;
    char* esc_seq = (char*)sh_malloc(ESC_SEQUENCE_MAX_LEN);

    while (TRUE)
    {
        input = input_new();
        write_prompt_basename();

        while ((non_canon_curr = get_key_pressed()) != ENTER_CODE)
        {
            switch (non_canon_curr)
            {
                case ARROW_UP:
                    if (!history->curr_watching) history->curr_watching = history->head;
                    else if (history->curr_watching->next) history->curr_watching = history->curr_watching->next;
                    
                    if (history->curr_watching) {
                        char tmp[history->curr_watching->command->length + ESC_SEQUENCE_MAX_LEN];
                        strcpy(tmp, history->curr_watching->command->content);
                        previous_watched_command_len = input->size;
                        
                        if (previous_watched_command_len > 0) {
                            sprintf(esc_seq, "\033[%dD", previous_watched_command_len);
                            char_array_prepend(tmp, esc_seq);
                        }

                        if (previous_watched_command_len > 0)
                            write(1, tmp, history->curr_watching->command->length + strlen(esc_seq));
                        else
                            write(1, tmp, history->curr_watching->command->length);
                        cursor_pos_in_command += history->curr_watching->command->length;

                        input_buffer_pop_last_n(input, previous_watched_command_len);
                        input_buffer_push_string(input, history->curr_watching->command->content, history->curr_watching->command->length);

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
                    if (history->curr_watching) history->curr_watching = history->curr_watching->prev;
                    previous_watched_command_len = input->size;

                    if (history->curr_watching) {
                        char tmp[history->curr_watching->command->length + ESC_SEQUENCE_MAX_LEN];
                        strcpy(tmp, history->curr_watching->command->content);

                        if (previous_watched_command_len > 0) {
                            sprintf(esc_seq, "\033[%dD", previous_watched_command_len);
                            char_array_prepend(tmp, esc_seq);
                        }
                        write(1, tmp, history->curr_watching->command->length + strlen(esc_seq));
                        input_buffer_pop_last_n(input, previous_watched_command_len);
                        input_buffer_push_string(input, history->curr_watching->command->content, history->curr_watching->command->length);

                        if (history->curr_watching->command->length < previous_watched_command_len) {
                            for (int i = history->curr_watching->command->length; i < previous_watched_command_len; i++) {
                                char space = SPACE_SYMBOL;
                                write(1, &space, 1);
                            }
                            sprintf(esc_seq, "\033[%dD", previous_watched_command_len - history->curr_watching->command->length);
                            write(1, esc_seq, strlen(esc_seq));
                        }
                    }
                    else {
                        for (int i = 0; i < previous_watched_command_len; i++) {
                            write(1, "\033[1D \033[1D", strlen("\033[1D \033[1D"));
                        }
                        input_buffer_pop_last_n(input, previous_watched_command_len);
                        cursor_pos_in_command = 0;
                    }
                    break;
                case ARROW_LEFT:
                    if (cursor_pos_in_command > 0) {
                        write(1, "\033[1D", strlen("\033[1D"));
                        cursor_pos_in_command--;
                    }
                    break;
                case ARROW_RIGHT:
                    if (cursor_pos_in_command < input->size) {
                        write(1, "\033[1C", strlen("\033[1C"));
                        cursor_pos_in_command++;
                    }
                    break;
                case BACKSPACE:
                    if (input->size > 0) {
                        write(1, "\033[1D \033[1D", strlen("\033[1D \033[1D"));
                        input_buffer_pop(input);
                    }
                    cursor_pos_in_command--;
                    break;
                default:
                    if (cursor_pos_in_command < input->size) {
                        write(1, &non_canon_curr, 1);
                        for (int i = cursor_pos_in_command; i < input->size; i++) {
                            write(1, &input->buffer[i], 1);
                        }
                        sprintf(esc_seq, "\033[%dD", input->size - cursor_pos_in_command);
                        write(1, esc_seq, strlen(esc_seq));

                        input_buffer_push_at(input, non_canon_curr, cursor_pos_in_command);
                        cursor_pos_in_command++;
                    } else {
                        write(1, &non_canon_curr, 1);
                        input_buffer_push(input, non_canon_curr);
                        cursor_pos_in_command++;
                    }
                    break;
            }
        }
        cursor_pos_in_command = 0;
        if (!input->size) { printf("\n"); continue; }

        putchar(non_canon_curr);

        // check if the prompt basename has been changed. If so, we should free memory that
        // has been allocated for a new basename to avoid memory leaks
        if (!strcmp(prompt_basename, DEFAULT_PROMPT_BASENAME)) free_prompt_base = 1;
        if (!strcmp(hist_file_name, DEFAULT_HIST_FILENAME)) free_hist_filename = 1;
        if (!strcmp(input->buffer, "exit")) { input_free(input); break; }

        input_buffer_push(input, '\0');
        cmd = command_parse_new(input->buffer);
        if (cmd) {
            shell_history_prepend(history, cmd);
            int status = command_handle(cmd);
        }

        input_free(input);
    }
    command_free(cmd);
    shell_history_free(history);

    if (free_prompt_base) free(prompt_basename);
    if (free_hist_filename) free(hist_file_name);

    free(session_id);

    return 0;
}