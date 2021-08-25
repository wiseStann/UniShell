#include "../include/common.h"
#include "../include/pipeline.h"


/*
 *
 */
int
command_handle(command_t* command)
{
    COMMANDS_ALIASES cmd_alias = (COMMANDS_ALIASES)(command->table_index);
    char** args = command_args_to_string_array(command);

    int ret;
    if (fork() == 0) {
        switch (cmd_alias) {
            case CMDS_LIST_CMD:
                commands_show();
                break;
            case CHANGE_PB_CMD:
                if (command->args_num - 1)
                    set_prompt_basename(command->arguments[1]->name);
                else
                    usage(command);
                break;
            default:
                if (command->args_num - 1)
                    return execvp(command->name, args);
                else 
                    return execlp(command->name, command->name, (char*)NULL);
        }
    }
    wait(NULL);
    string_array_free(args, command->args_num);

    return STATUS_SUCCESS;
}