#include "include/common.h"
#include "include/input_buffer.h"
#include "include/command.h"
#include "include/parser.h"
#include "include/sh_history.h"
#include "include/utils.h"
#include "include/pipeline.h"
#include "include/interact.h"




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
    char* ptr;

    while (TRUE)
    {
        input = input_new();
        write_prompt_basename();

        while ((non_canon_curr = get_key_pressed()) != ENTER_CODE)
            handle_keyboard_input(input, &cursor_pos_in_command, non_canon_curr);

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
            if (!status) printf("Pipeline cannot be handled");
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