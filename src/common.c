#include "../include/common.h"
#include "../include/parser.h"
#include "../include/sh_history.h"


char* prompt_basename = NULL;
history_t* history = NULL;
char* hist_file_name = NULL;
char* session_id = NULL;

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
    { .cmd_name = "exit", .description = "exits this applications" },
    { .cmd_name = "chgpb", .description = "changes the prompt basename to a new one" },
    { .cmd_name = "cmdslist", .description = "shows available commands" },
    { .cmd_name = "hist", .description = "shows shell history" },
    { .cmd_name = "histsave", .description = "saves current history to a UniShell history file" },
    { .cmd_name = "histload", .description = "load shell history from the file by a given id" },
    { .cmd_name = "histfree", .description = "frees session history" },
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
set_session_id()
{
    char unique_id[100];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    strftime(unique_id, sizeof(unique_id) - 1, "%d%m%Y%H%M", t);
    session_id = (char*)sh_malloc(SESSION_ID_MAX_LEN);
    strcpy(session_id, unique_id);
}


/*
 *
 */
void
set_prompt_basename(const char* _basename)
{
    int basename_length = strlen(_basename);
    if (basename_length) {
        if (prompt_basename != NULL)
            free(prompt_basename);
        prompt_basename = (char*)sh_malloc(basename_length + 1);   
        strcpy(prompt_basename, _basename);
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
{ write(1, prompt_basename, strlen(prompt_basename)); }

/*
 *
 */
void
set_history_filename(const char* _filename)
{
    int his_filename_length = strlen(_filename);
    if (his_filename_length) {
        if (hist_file_name != NULL)
            free(hist_file_name);
        hist_file_name = (char*)sh_malloc(his_filename_length + 1);   
        strcpy(hist_file_name, _filename);
        hist_file_name[his_filename_length] = SPACE_SYMBOL;
        hist_file_name[his_filename_length + 1] = '\0';
    }
    else
        throw_error("History filename must be at least one character long, skipped\n");
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