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
char getch() {
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
		perror ("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror ("tcsetattr ~ICANON");
	return buf;
}

/*
 *
 */
int get_key_pressed()
{
    int getch_out;
    getch_out = getch();
    if (getch_out == ESCAPE_CODE) {
        getch();
        switch(getch())
        {
            case A_KEY:
                return ARROW_UP;
            case B_KEY:
                return ARROW_DOWN;
            // case C_KEY:
            //
            case D_KEY:
                return BACKSPACE;
        }
    }
    return getch_out;
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
void char_array_prepend(char* s, const char* t)
{
    size_t len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    memcpy(s, t, len);
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