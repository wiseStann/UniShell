#include "../include/parser.h"



/*
 *
 */
command_t*
command_parse_new(char* command)
{
    command_t* struct_command = command_new(command);
    if (!struct_command) return NULL;

    int cmd_name_len = strlen(struct_command->name);

    int args_idx = 0, next_token_idx = 0, quote_is_completed = 1, i = 0;
    argument_t* argument = command_argument_new(next_token_idx);
    char* tmp = strdup(command);

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
        argument = command_argument_new(next_token_idx);

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
command_parse_is_valid(const char* command)
{
    char* basename = command_parse_get_basename(command);
    int ex_factor = command_parse_basename_exists(basename);
    int val_syntax_factor = command_parse_syntax_is_valid(command);
    if (!ex_factor) {
        throw_error("! Unknow command '%s'\n", basename);
        throw_error("! Commands list is available by typing 'cmdslist'\n");
    }
    else if (!val_syntax_factor)
        throw_error("! Invalid command syntax\n");
    
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
    return args_num;
}

/*
 *
 */
char*
command_parse_get_basename(const char* command)  
{
    char* basename = (char*)sh_malloc(COMMAND_NAME_MAX_LEN);
    int i;
    for (i = 0; command[i] != ' ' && command[i] != 0; i++) {
        basename[i] = command[i];
    }
    basename[i] = '\0';
    return basename;
}