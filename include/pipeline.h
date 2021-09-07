#ifndef PIPELINE_SHELL_H
#define PIPELINE_SHELL_H

#include "command.h"

#define DEFAULT_PIPELINE_COMMANDS_CAP 16
#define PIPELINE_COMMANDS_CAP_EXP_VAL 1.5


// pipeline commands structure
typedef struct pipeline_commands {
    command_t** cmds_list;
    unsigned capacity;
    unsigned size;
} pipe_cmds_t;

//
pipe_cmds_t* pipeline_commands_new();

//
void pipeline_commands_push(pipe_cmds_t*, command_t*);

//
void pipeline_commands_reallocate(pipe_cmds_t*, unsigned);

//
void pipeline_commands_free(pipe_cmds_t*);


// handles a given command and produces corresponding output
int pipeline_commands_handle(pipe_cmds_t*);

//
int pipeline_pipes_handle(pipe_cmds_t*);


#endif // !PIPELINE_SHELL_H