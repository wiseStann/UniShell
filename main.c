#include "include/common.h"
#include "include/input_buffer.h"
#include "include/command.h"
#include "include/parser.h"
#include "include/sh_history.h"
#include "include/utils.h"
#include "include/pipeline.h"




/*
        PIPES HANDLING
        UNKNOWN COMMANDS HANDLING WITH EXECVP
        WRITE CURSOR MOVEMENT MACROSES
        STREAM REDIRECTION HANDLING
        FIX EXIT, I THINK THE REASON IS OTHER PROCESSES CREATED IN HANDLERS
        CREATE A WINDOW FOR THE UNISHELL
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
    pipe_cmds_t* pipeline_commands = NULL;

    int free_prompt_base = 0, free_hist_filename = 0;
    int cursor_pos_in_command = 0, non_canon_curr;
    unsigned previous_watched_command_len;
    char esc_seq[ESC_SEQUENCE_MAX_LEN], *ptr;

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
                            CURSOR_MOVE_LEFT(previous_watched_command_len - history->curr_watching->command->length);
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
                            CURSOR_MOVE_LEFT(previous_watched_command_len - history->curr_watching->command->length);
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
                        CURSOR_MOVE_LEFT(1);
                        cursor_pos_in_command--;
                    }
                    break;
                case ARROW_RIGHT:
                    if (cursor_pos_in_command < input->size) {
                        CURSOR_MOVE_RIGHT(1);
                        cursor_pos_in_command++;
                    }
                    break;
                case BACKSPACE:
                    if (input->size > 0) {
                        input_buffer_pop_at(input, --cursor_pos_in_command);
                        write(1, "\033[1D \033[1D", strlen("\033[1D \033[1D"));
                        
                        if (cursor_pos_in_command != input->size) {
                            for (int i = cursor_pos_in_command; i < input->size; i++) {
                                write(1, &input->buffer[i], 1);
                            }

                            write(1, " \033[1D", strlen(" \033[1D"));
                            CURSOR_MOVE_LEFT(input->size - cursor_pos_in_command);
                        }
                    }
                    break;
                default:
                    if (cursor_pos_in_command < input->size) {
                        write(1, &non_canon_curr, 1);
                        for (int i = cursor_pos_in_command; i < input->size; i++) {
                            write(1, &input->buffer[i], 1);
                        }
                        
                        CURSOR_MOVE_LEFT(input->size - cursor_pos_in_command);

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
        input_buffer_push(input, '\0');

        // check if the prompt basename has been changed. If so, we should free memory that
        // has been allocated for a new basename to avoid memory leaks
        if (!strcmp(prompt_basename, DEFAULT_PROMPT_BASENAME)) free_prompt_base = 1;
        if (!strcmp(hist_file_name, DEFAULT_HIST_FILENAME)) free_hist_filename = 1;
        if (!strcmp(input->buffer, "exit")) { input_free(input); goto cleaning_exit; }

        cmd = command_parse_new(input->buffer, FALSE);
        shell_history_prepend(history, cmd);

        // if the whole command is valid then it is splitted by pipes
        // and joined in a chain of subcommands
        if (cmd->is_valid) {
            char* buffer = strdup(input->buffer);
            pipeline_commands = pipeline_commands_new();
            do {
                cmd = command_parse_new(buffer, TRUE);

                ptr = strchr(buffer, VERTICAL_BAR_SYMBOL);
                if (ptr) buffer = ptr + 1;
                else buffer += strlen(buffer);

                if (cmd->is_valid) {
                    pipeline_commands_push(pipeline_commands, cmd);
                }
            } while (*buffer);
            int status = pipeline_commands_handle(pipeline_commands);
            pipeline_commands_free(pipeline_commands);
        }
        input_free(input);
    }

cleaning_exit:
    shell_history_free(history);

    if (free_prompt_base) free(prompt_basename);
    if (free_hist_filename) free(hist_file_name);

    free(session_id);

    return 0;
}