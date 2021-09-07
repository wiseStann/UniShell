#include "../include/common.h"
#include "../include/utils.h"
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
command_new(char* command, int pipes_visible)
{
    command_t* struct_command;
    argument_t* argument;
    unsigned int cmd_args_num;
    
    struct_command = (command_t*)sh_malloc(sizeof(command_t));

    command_trim(command);
    struct_command->length = command_length(command, pipes_visible);

    // allocate len + ESC_SEQUENCE_MAX_LEN bytes in order to expand contents with paticular
    // escape sequence in the future
    struct_command->content = (char*)sh_malloc(struct_command->length + ESC_SEQUENCE_MAX_LEN);
    strcpy(struct_command->content, char_array_substring(command, 0, struct_command->length));

    struct_command->name = (char*)sh_malloc(COMMAND_NAME_MAX_LEN);
    struct_command->name = command_parse_get_basename(struct_command);

    struct_command->table_index = commands_array_get_index(AVAILABLE_COMMANDS, struct_command->name);

    cmd_args_num = command_parse_arguments_number(struct_command);
    struct_command->args_num = cmd_args_num;
    if (cmd_args_num) {
        struct_command->arguments = (argument_t**)sh_malloc(sizeof(argument_t*));
        for (int i = 0; i < cmd_args_num; i++) {
            struct_command->arguments[i] = (argument_t*)sh_malloc(sizeof(argument_t));
        }
    }

    struct_command->is_valid = command_parse_is_valid(struct_command);

    return struct_command;
}

/*
 *
 */
unsigned command_length(const char* command, int pipes_visible)
{
    char* ptr, *substr;
    unsigned length;

    ptr = strchr(command, VERTICAL_BAR_SYMBOL);
    if (!ptr || !pipes_visible)
        return strlen(command);
    
    substr = char_array_substring(command, 0, ptr - command);
    char_array_trim(substr);

    length = strlen(substr);
    free(substr);

    return length;
}

/*
 *
 */
void command_trim(char* command)
{
    char_array_trim(command);
}

/*
 *
 */
void command_info(command_t* command)
{
    printf("command name: '%s'\n", command->name);
    printf("command content: '%s'\n", command->content);
    printf("command length = %d\n", command->length);
    printf("command args num: %d\n", command->args_num);
    for (int i = 0; i < command->args_num; i++) {
        printf("Arg %d: '%s'\n", i, command->arguments[i]->name);
    }
    printf("\n");
}

/*
 *
 */
void
command_free(command_t* command)
{
    if (command) {
        for (int i = 0; i < command->args_num; i++) 
            free(command->arguments[i]);
        free(command->arguments);
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
        args = (char**)sh_malloc((command->args_num + 1) * sizeof(char*));
        for (i = 0; i < command->args_num; i++)
            args[i] = (char*)sh_malloc(COMMAND_ARGUMENT_MAX_LEN);

        for (i = 0; i < command->args_num; i++) {
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