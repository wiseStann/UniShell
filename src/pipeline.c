#include "../include/common.h"
#include "../include/utils.h"
#include "../include/pipeline.h"
#include "../include/sh_history.h"

/*
 *
 */
int
command_handle(command_t* command)
{
    COMMANDS_ALIASES cmd_alias = (COMMANDS_ALIASES)(command->table_index);
    char** args = command_args_to_string_array(command);
    int status;

    int ret;
    if (fork() == 0) {
        switch (cmd_alias) {
            case CMDS_LIST_CMD:
                commands_show();
                break;
            case CHANGE_PB_CMD:
                if (command->args_num > 1)
                    set_prompt_basename(command->arguments[1]->name);
                else
                    usage(command);
                break;
            case HIST_SHOW_CMD:
                shell_history_show(history);
                break;
            case HIST_SAVE_CMD:
                status = file_save_history(history);
                if (status == STATUS_FAILURE)
                    throw_error("! Unable to open the file to save history\n");
                break;
            case HIST_LOAD_CMD:
                if (command->args_num == 2) {
                    status = file_load_history(command->arguments[1]->name);
                    if (status == STATUS_FAILURE)
                        throw_error("! Unable to open the file by a given id to load history\n");
                } else
                    usage(command);
                break;
            case HIST_FREE_CMD:
                shell_history_free(history);
                history = shell_history_new();
                break;
            default:
                return execvp(command->name, args);
        }
    }
    wait(NULL);
    string_array_free(args, command->args_num);

    return STATUS_SUCCESS; 
}