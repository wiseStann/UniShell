#include "../include/common.h"
#include "../include/utils.h"
#include "../include/pipeline.h"
#include "../include/sh_history.h"


/*
 *
 */
pipe_cmds_t*
pipeline_commands_new()
{
    pipe_cmds_t* pcmds = (pipe_cmds_t*)sh_malloc(sizeof(pipe_cmds_t));
    pcmds->cmds_list = (command_t**)sh_malloc(sizeof(command_t*));
    pcmds->capacity = DEFAULT_PIPELINE_COMMANDS_CAP;
    // for (int i = 0; i < pcmds->capacity; i++) {
        // pcmds->cmds_list[i] = (command_t*)sh_malloc(sizeof(command_t));
    // }
    pcmds->size = 0;

    return pcmds;
}

/*
 *
 */
void
pipeline_commands_push(pipe_cmds_t* commands, command_t* cmd)
{
    if (commands) {
        if (commands->size >= commands->capacity)
            pipeline_commands_reallocate(commands, commands->capacity * PIPELINE_COMMANDS_CAP_EXP_VAL);

        commands->cmds_list[commands->size++] = cmd;
    }
}

/*
 *
 */
void
pipeline_commands_reallocate(pipe_cmds_t* commands, unsigned bytes)
{
    commands->cmds_list = realloc((void*)commands->cmds_list, bytes);
    commands->capacity = bytes;
}

/*
 *
 */
void
pipeline_commands_free(pipe_cmds_t* commands)
{
    if (commands) {
        for (int i = 0; i < commands->size; i++) {
            free(commands->cmds_list[i]);
        }
        free(commands->cmds_list);
        free(commands);
    }
}

/*
 *
 */
int
pipeline_commands_handle(pipe_cmds_t* commands)
{
    if (commands->size == 1) {
        command_t* whole_command = commands->cmds_list[0];
        COMMANDS_ALIASES cmd_alias = (COMMANDS_ALIASES)(whole_command->table_index);
        char** args = command_args_to_string_array(whole_command);
        int status;

        int ret;

#ifdef __unix__

        if (fork() == 0) {
            switch (cmd_alias) {
                case CMDS_LIST_CMD:
                    commands_show();
                    break;
                case CHANGE_PB_CMD:
                    if (whole_command->args_num > 1)
                        set_prompt_basename(whole_command->arguments[1]->name);
                    else
                        usage(whole_command);
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
                    if (whole_command->args_num == 2) {
                        status = file_load_history(whole_command->arguments[1]->name);
                        if (status == STATUS_FAILURE)
                            throw_error("! Unable to open the file by a given id to load history\n");
                    } else
                        usage(whole_command);
                    break;
                case HIST_FREE_CMD:
                    shell_history_free(history);
                    history = shell_history_new();
                    break;
                default:
                    return execvp(whole_command->name, args);
            }
        }
        wait(NULL);

#elif _WIN32

#endif
        printf("\nCommand has been successfully handled\n");
        string_array_free(args, whole_command->args_num);
    }
    else {
        pipeline_pipes_handle(commands);
    }

    return STATUS_SUCCESS;
}

/*
 *
 */
int
pipeline_pipes_handle(pipe_cmds_t* pipe_commands)
{
	int pipefd[2];
    command_t* comm1 = pipe_commands->cmds_list[0];
    char** args1 = command_args_to_string_array(comm1);
    command_t* comm2 = pipe_commands->cmds_list[1];
    char** args2 = command_args_to_string_array(comm2);
    //pipe(pipefd);
    //if (fork() == 0) {
    //    //child
    //    //write
    //    close(pipefd[0]);
    //    dup2(pipefd[1], STDOUT_FILENO);
    //    execvp(comm1->name, args1);
    //} else {  
    //    //parent
    //    close(pipefd[1]);
    //    dup2(pipefd[0], STDIN_FILENO);
    //    execvp(comm2->name, args2);
    //}
    printf("Pipes have been successfully handled\n");

    return 0;
}