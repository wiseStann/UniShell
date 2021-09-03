#include "../include/utils.h"


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

    // INPUT UNTILS //

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
            case C_KEY:
                return ARROW_RIGHT;
            case D_KEY:
                return ARROW_LEFT;
            // case D_KEY:
                // return BACKSPACE;
        }
    }
    return getch_out;
}

/*
 *
 */

