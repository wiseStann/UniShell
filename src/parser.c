#include "../include/common.h"
#include "../include/parser.h"


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
    if (!command_parse_is_valid(command)) {
        return NULL;
    }

    command_t* struct_command;
    argument_t* argument;
    unsigned int cmd_args_num;
    
    struct_command = (command_t*)malloc(sizeof(command_t));
    struct_command->name = (char*)malloc(COMMAND_NAME_MAX_LEN);
    struct_command->name = command_parse_get_basename(command);

    struct_command->table_index = commands_array_get_index(AVAILABLE_COMMANDS, struct_command->name);

    struct_command->length = strlen(command);

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
command_parse_new(char* command)
{
    command_t* struct_command = command_new(command);
    if (!struct_command) return NULL;

    int cmd_name_len = strlen(struct_command->name);
    command += cmd_name_len;

    int args_idx = 0, next_token_idx = 0, quote_is_completed = 1, i = 0;
    argument_t* argument = command_argument_new();
    char* tmp = malloc(strlen(command));
    strcpy(tmp, command);

    char* next_token = strtok(command, " ");
    while (next_token)
    {
        next_token_idx++;
        while (*tmp == SPACE_SYMBOL) tmp++;

        if (char_array_contains(next_token, COMMENT_SYMBOL)) {
            while (next_token[i] != COMMENT_SYMBOL)
                argument->name[argument->size++] = next_token[i++];
        } else if (next_token[0] == DOUBLE_QUOTE_SYMBOL && quote_is_completed) {
            next_token = strtok(NULL, " ");
            while (next_token[strlen(next_token) - 1] != DOUBLE_QUOTE_SYMBOL)
                next_token = strtok(NULL, " ");
            do {
                tmp++;
                argument->name[argument->size++] = *tmp;
            } while (*(tmp + 1) != DOUBLE_QUOTE_SYMBOL);
            argument->name[argument->size] = TERMINAL_SYMBOL;
        } else {
            tmp += strlen(next_token);
            strcpy(argument->name, next_token);
            argument->size = strlen(next_token);
        }
        argument->idx_number = next_token_idx;
        struct_command->arguments[args_idx++] = argument;
        argument = command_argument_new();

        if (next_token[i] == COMMENT_SYMBOL) break;
        next_token = strtok(NULL, " ");
    }
    free(argument);

    return struct_command;
}

/*
 *
 */
int
command_handle(command_t* command)
{
    COMMANDS_ALIASES cmd_alias = (COMMANDS_ALIASES)(command->table_index);
    switch (cmd_alias) {
        case ECHO_CMD:
            printf("Will be released soon.\n");
            break;
        case LS_CMD:
            printf("Will be released soon.\n");
            break;
        case PWD_CMD:
            printf("Will be released soon.\n");
            break;
        case CLEAR_CMD:
            printf("Will be released soon.\n");
            break;
        case CMDS_LIST_CMD:
            commands_show();
            break;
        default:
            return STATUS_FAILURE;
            break;
    }
    return STATUS_SUCCESS;
}

/*
 *
 */
int
command_parse_is_valid(const char* command)
{
    char* basename = command_parse_get_basename(command);
    int ex_factor = command_parse_basename_exists(basename);
    int val_syntax_factor = command_parse_syntax_is_valid(command);
    if (!ex_factor) {
        printf("! Unknow command '%s'\n", basename);
        printf("! Commands list is available by typing 'cmdslist'\n");
    }
    else if (!val_syntax_factor)
        printf("! Invalid command syntax\n");
    
    free(basename);
    return ex_factor && val_syntax_factor;
}

/*
 *
 */
int
command_parse_basename_exists(const char* basename)
{
    return commands_array_contains(AVAILABLE_COMMANDS, basename);
}

/*
 *
 */
int
command_parse_syntax_is_valid(const char* command)
{
    char curr, prev;
    unsigned int quotes_num = 0, quote_is_completed = 1;
    for (int i = 0; command[i] != 0; i++) {
        curr = command[i];
        if (curr == DOUBLE_QUOTE_SYMBOL) {
            if (quote_is_completed && prev != SPACE_SYMBOL) return FALSE;
            quote_is_completed = (quote_is_completed + 1) % 2;
            quotes_num++;
        }
        if (prev == DOUBLE_QUOTE_SYMBOL && quote_is_completed && curr != SPACE_SYMBOL)
            return FALSE;
        if (curr == FORWARD_SLASH_SYMBOL)
            return FALSE;
        if (!char_array_contains(SPEC_SYMBOLS, curr) &&
            (curr < 65 || curr > 90) && (curr < 97 || curr > 122))
            return FALSE;
        if (curr == COMMENT_SYMBOL && !quote_is_completed)
            return FALSE;
        prev = curr;
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
    char curr, prev;
    unsigned int args_num = 0, quote_is_completed = 1;
    unsigned int cmd_len = strlen(command);
    for (int i = 0; i <= cmd_len; i++) {
        curr = command[i];
        
        if (curr == SPACE_SYMBOL && prev == SPACE_SYMBOL) continue;
        if ((curr == SPACE_SYMBOL || curr == TERMINAL_SYMBOL) && prev != SPACE_SYMBOL) {
            args_num++;
        }
        else if (curr == DOUBLE_QUOTE_SYMBOL) {
            if (quote_is_completed) {
                do {
                    i++;
                } while (command[i] != DOUBLE_QUOTE_SYMBOL);
            }
            quote_is_completed = (quote_is_completed + 1) % 2;
        } else if (curr == COMMENT_SYMBOL) {
            args_num++;
            break;
        }
        prev = curr;
    }
    return args_num - 1;
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

/*
 *
 */
void command_free(command_t* command)
{
    if (command) {
        if (command->args_num) {
            for (int i = 0; i < command->args_num; i++) 
                free(command->arguments[i]);
            free(command->arguments);
        }
        free(command->name);
        free(command);
    }
}