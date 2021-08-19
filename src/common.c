#include "../include/common.h"
#include "../include/parser.h"
#include "../include/sh_history.h"


char* prompt_basename = NULL;


/*
 *
 */
void
throw_error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
 
    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd') {
                int i = va_arg(args, int);
                printf("%d", i);
            } else if (*fmt == 'c') {
                int c = va_arg(args, int);
                printf("%c", c);
            } else if (*fmt == 'f') {
                double d = va_arg(args, double);
                printf("%f", d);
            } else if (*fmt == 's') {
                char* arg = (char*)sh_malloc(COMMAND_NAME_MAX_LEN);
                strcpy(arg, va_arg(args, char*));
                printf("%s", arg);
                free(arg);
            }
        } else
            printf("%c", *fmt);
    
        fmt++;
    }
    va_end(args);
}



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

    struct_command->table_index = commands_array_get_index(AVAILABLE_COMMANDS, struct_command->name);

    struct_command->length = strlen(command);

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

    if (command->args_num - 1) {
        args = (char**)sh_malloc(++command->args_num * sizeof(char*));
        for (int i = 0; i < command->args_num - 1; i++)
            args[i] = (char*)sh_malloc(COMMAND_ARGUMENT_MAX_LEN);

        for (int i = 0; i < command->args_num - 1; i++) {
            strcpy(args[i], command->arguments[i]->name);
        }
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


// available commands
avaliable_cmd_entry_t AVAILABLE_COMMANDS[COMMANDS_MAX_NUM] = {
    { .cmd_name = "echo", .description = "prints something to console or to the I/O streams" },
    { .cmd_name = "ls", .description = "shows all files and dirs in the current directory" },
    { .cmd_name = "cat", .description = "prints the data from the stream to the console" },
    { .cmd_name = "pwd", .description = "shows path to the current directory" },
    { .cmd_name = "clear", .description = "clears the screen" },
    { .cmd_name = "exit", .description = "exits this applications" },
    { .cmd_name = "chgpb", .description = "changes the prompt basename to a new one" },
    { .cmd_name = "cmdslist", .description = "shows available commands" },
    { NULL, NULL },
};

const char SPEC_SYMBOLS[SPEC_SYMBOLS_MAX_NUM] = {
    '#', ' ', '\"', '\\', '/', '|', '>', '<', '-', '.', '$', '\0',
};

/*
 *
 */
int
char_array_contains(const char* array, char symbol)
{
    for (int i = 0; array[i]; i++) {
        if (array[i] == symbol)
            return TRUE;
    }
    return FALSE;
}

/*
 *
 */
int
commands_array_contains(avaliable_cmd_entry_t* entries, const char* string)
{
    for (int i = 0; entries[i].cmd_name; i++) {
        if (!strcmp(entries[i].cmd_name, string)) {
            return TRUE;
        }
    }
    return FALSE;
}

/*
 *
 */
int
commands_array_get_index(avaliable_cmd_entry_t* entries, const char* string)
{
    for (int i = 0; entries[i].cmd_name; i++) {
        if (!strcmp(entries[i].cmd_name, string)) {
            return i;
        }
    }
    return -1;
}

/*
 *
 */
void string_array_free(char** str_array, unsigned int length)
{
    if (str_array) {
        for (int i = 0; i < length; i++)
            free(str_array[i]);
        free(str_array);
    }
}


/*
 *
 */
void*
sh_malloc(unsigned int bytes)
{
    void* memblock = malloc(bytes);
    if (!memblock) {
        perror("Error occured during mem allocation");
        exit(EXIT_FAILURE);
    }
    return memblock;
}


/*
 *
 */
void
set_prompt_basename(const char* basename)
{
    int basename_length = strlen(basename);
    if (basename_length) {
        if (prompt_basename != NULL)
            free(prompt_basename);
        prompt_basename = (char*)sh_malloc(basename_length + 1);   
        strcpy(prompt_basename, basename);
        prompt_basename[basename_length] = SPACE_SYMBOL;
        prompt_basename[basename_length + 1] = '\0';
    }
    else
        throw_error("Prompt basename must be at least one character long, skipped\n");
}

/*
 *
 */
void
commands_show()
{
    printf("Available commands:\n");
    for (int i = 0; AVAILABLE_COMMANDS[i].cmd_name; i++) {
        printf("-> Name: %s\n", AVAILABLE_COMMANDS[i].cmd_name);
        printf("   Description: %s\n", AVAILABLE_COMMANDS[i].description);
    }
}