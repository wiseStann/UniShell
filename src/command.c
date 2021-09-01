#include "../include/common.h"
#include "../include/command.h"
#include "../include/parser.h"

/*
 *
 */
argument_t*
command_argument_new(unsigned int argument_index)
{
    argument_t* argument = (argument_t*)sh_malloc(sizeof(argument_t));
    argument->size = 0;
    argument->idx_number = argument_index;
    return argument;
}

/*
 *
 */
command_t*
command_new(const char* command)
{
    if (!command_parse_is_valid(command)) {
        return NULL;
    }

    command_t* struct_command;
    argument_t* argument;
    unsigned int cmd_args_num;
    
    struct_command = (command_t*)sh_malloc(sizeof(command_t));
    struct_command->name = (char*)sh_malloc(COMMAND_NAME_MAX_LEN);
    struct_command->name = command_parse_get_basename(command);

    struct_command->length = strlen(command);

    // allocate len + ESC_SEQUENCE_MAX_LEN bytes in order to expand contents with paticular
    // escape sequence in the future
    struct_command->content = (char*)sh_malloc(struct_command->length + ESC_SEQUENCE_MAX_LEN);
    strcpy(struct_command->content, command);

    struct_command->table_index = commands_array_get_index(AVAILABLE_COMMANDS, struct_command->name);

    cmd_args_num = command_parse_arguments_number(command);
    struct_command->args_num = cmd_args_num;

    if (cmd_args_num) {
        struct_command->arguments = (argument_t**)sh_malloc(sizeof(argument_t*));
        for (int i = 0; i < cmd_args_num; i++) {
            struct_command->arguments[i] = (argument_t*)sh_malloc(sizeof(argument_t));
        }
    }

    return struct_command;
}

/*
 *
 */
void
command_free(command_t* command)
{
    if (command) {
        if (command->args_num) {
            for (int i = 0; i < command->args_num; i++) 
                free(command->arguments[i]);
            free(command->arguments);
        }
        free(command->name);
        free(command->content);
        free(command);
    }
}

/*
 *
 */
char**
command_args_to_string_array(command_t* command)
{
    char** args;
    int i;

    if (command->args_num) {
        args = (char**)sh_malloc(++command->args_num * (sizeof(char*) + 1));
        for (i = 0; i < command->args_num - 1; i++)
            args[i] = (char*)sh_malloc(COMMAND_ARGUMENT_MAX_LEN);

        for (i = 0; i < command->args_num - 1; i++) {
            strcpy(args[i], command->arguments[i]->name);
        }
        args[i] = (char*)NULL;
    }

    return args;
}

/*
 *
 */
void
usage(command_t* command)
{

}