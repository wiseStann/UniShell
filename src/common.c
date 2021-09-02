#include "../include/common.h"
#include "../include/parser.h"
#include "../include/sh_history.h"


char* prompt_basename = NULL;
history_t* history = NULL;


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
    { .cmd_name = "hist", .description = "shows shell history" },
    { NULL, NULL },
};

const char SPEC_SYMBOLS[SPEC_SYMBOLS_MAX_NUM] = {
    '#', ' ', '\"', '\\', '/', '|', '>', '<', '-', '.', '$', '\0',
};


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
write_prompt_basename()
{
    write(1, prompt_basename, strlen(prompt_basename));
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