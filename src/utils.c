#include "../include/utils.h"
#include "../include/parser.h"
#include "../include/sh_history.h"

    // ARRAYS UTILS //
    
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
void
char_array_prepend(char* s, const char* t)
{
    size_t len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    memcpy(s, t, len);
}

/*
 *
 */
void
char_array_append(char* s, const char* t)
{
    size_t len = strlen(t);
    strncat(s, t, len);
}

/*
 *
 */
void char_array_trim(char* s)
{
    char* contents = s;
    int len = strlen(contents);

    while(isspace(contents[len - 1]))
        contents[--len] = 0;
    while(*contents && isspace(*contents))
        ++contents, --len;

    memmove(s, contents, len + 1);
}

/*
 *
 */
char* char_array_substring(const char* s, unsigned start, unsigned end)
{
    char* substr = (char*)sh_malloc(strlen(s));
    memcpy(substr, &s[start], end);
    substr[end] = '\0';
    return substr;
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
void
string_array_free(char** str_array, unsigned int length)
{
    if (str_array) {
        for (int i = 0; i < length; i++)
            free(str_array[i]);
        free(str_array);
    }
}


    // INPUT UNTILS //

#ifdef __unix__

/*
 *
 */
char
getch() {
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

#endif

/*
 *
 */
int
get_key_pressed()
{
    int init_buffer, ch = -1;
    init_buffer = getch();

    //printf("\nInit buffer: %d", init_buffer);

    // in different machines backspace can be 8 or 127 coded
    if (init_buffer == BS_KEY || init_buffer == BS_KEY2)
        return BACKSPACE;

#ifdef __unix__

    if (init_buffer == ESCAPE_CODE) {
        getch();
        ch = getch();
    }

#elif _WIN32

    if (init_buffer == WIN_META_CODE1 || init_buffer == WIN_META_CODE2)
        ch = getch();

#endif

    switch (ch)
    {
        case A_KEY:
            return ARROW_UP;
        case B_KEY:
            return ARROW_DOWN;
        case C_KEY:
            return ARROW_RIGHT;
        case D_KEY:
            return ARROW_LEFT;
        default: break;
    }

    return init_buffer;
}


    // FILE UTILS //

/*
 *
 */
int
file_save_history(history_t* history)
{
    char* filename = shell_history_get_save_filename(session_id);
    FILE* file = fopen(filename, "a");
    if (!file) return STATUS_FAILURE;

    his_entry_t* curr = history->head;
    while (curr) {
        fprintf(file, "%s\n", curr->command->content);
        curr = curr->next;
    }
    fclose(file);

    return STATUS_SUCCESS;
}

/*
 *
 */
int
file_load_history(const char* id)
{
    char* filename = shell_history_get_save_filename(id);
    FILE* file = fopen(filename, "r");
    if (!file) return STATUS_FAILURE;

    char* line = NULL;
    size_t length = 0;
    ssize_t read;
    command_t* cmd;
    shell_history_free(history);
    history = shell_history_new();
    while ((read = getline(&line, &length, file)) != -1) {
        line[strlen(line) - 1] = TERMINAL_SYMBOL;
        cmd = command_parse_new(line, FALSE);
        shell_history_append(history, cmd);
    }

    return STATUS_SUCCESS;
}