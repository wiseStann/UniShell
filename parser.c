#include "common.h"
#include "parser.h"


/*
 *
 */
argument_t*
command_argument_new()
{
    argument_t* argument = (argument_t*)malloc(sizeof(argument_t));
    argument->size = 0;
    return argument;
}

/*
 *
 */
command_t*
command_new(const char* command)
{
    if (!command_parse_is_valid(command))
        return NULL;

    command_t* struct_command;
    argument_t* argument;
    unsigned int cmd_args_num;

    struct_command = (command_t*)malloc(sizeof(command_t));
    struct_command->name = (char*)malloc(COMMAND_NAME_MAX_LEN);
    struct_command->name = command_parse_get_basename(command);

    cmd_args_num = command_parse_arguments_number(command);
    struct_command->args_num = cmd_args_num;

    if (cmd_args_num) {
        struct_command->arguments = (argument_t**)malloc(sizeof(argument_t*));
        for (int i = 0; i < cmd_args_num; i++) {
            struct_command->arguments[i] = (argument_t*)malloc(cmd_args_num * sizeof(argument_t));
        }
    }

    return struct_command;
}

/*
 *
 */
command_t*
command_parse_new(const char* command)
{
    command_t* struct_command = command_new(command);
    if (!struct_command) return NULL;

    argument_t* argument = command_argument_new();

    char prev;
    int arg_idx_start, args_idx = 0, quote_is_completed = 1, cmd_name_len = strlen(struct_command->name);
    for (int i = cmd_name_len; command[i] != 0; i++) {
        if (i == cmd_name_len)
            arg_idx_start = i;

        if (command[i] == SPACE_SYMBOL) {
            argument->name[i - arg_idx_start] = '\0';
            struct_command->arguments[args_idx++] = argument;
            arg_idx_start = i + 1;
            free(argument);
            argument = command_argument_new();
        } else if (command[i] == COMMENT_SYMBOL) {
            if (prev != SPACE_SYMBOL) {
                argument->name[i - arg_idx_start] = '\0';
                printf("%s\n", argument->name);
                struct_command->arguments[args_idx++] = argument;
                free(argument);
            }
            break;
        } else if (command[i] == DOUBLE_QUOTE_SYMBOL) {
            if (quote_is_completed)
                quote_is_completed = 0;
            else
                quote_is_completed = 1;
        } else {
            argument->name[argument->size++] = command[i];
        }
        prev = command[i];
    }

    return struct_command;
}

/*
 *
 */
int
command_parse_is_valid(const char* command)
{
    unsigned int quotes_num = 0;
    for (int i = 0; command[i] != 0; i++) {
        if (command[i] == DOUBLE_QUOTE_SYMBOL)
            quotes_num++;
        if (command[i] == FORWARD_SLASH_SYMBOL)
            return FALSE;
        if (!char_array_contains(SPEC_SYMBOLS, SPEC_SYMBOLS_NUM, command[i]) &&
            (command[i] < 65 || command[i] > 90) && (command[i] < 97 || command[i] > 122))
            return FALSE;
    }
    if (quotes_num % 2)
        return FALSE;

    return TRUE;
}

/*
 *
 */
unsigned int
command_parse_arguments_number(const char* command)
{
    unsigned int args_num = 0;
    for (int i = 0; command[i] != 0; i++) {
        if (command[i] == ' ') args_num++;
    }
    return args_num++;
}

/*
 *
 */
char*
command_parse_get_basename(const char* command)  
{
    char* basename = (char*)malloc(COMMAND_NAME_MAX_LEN);
    int i;
    for (i = 0; command[i] != ' ' && command[i] != 0; i++) {
        basename[i] = command[i];
    }
    basename[i] = '\0';
    return basename;
}