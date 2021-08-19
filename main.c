#include "include/common.h"
#include "include/parser.h"
#include "include/input_buffer.h"
#include "include/sh_history.h"


int main(int argc, char** argv)
{
    input_t* input = NULL;
    command_t* cmd = NULL;
    history_t* history = shell_history_new();
    char curr;

    set_prompt_basename(DEFAULT_PROMPT_BASENAME);

    while (TRUE) {
        input = input_new();
        printf("%s", prompt_basename);
        while ((curr = getchar()) != ENTER_CODE) {
            if (curr != BACK_SLASH_SYMBOL)
                input_buffer_push(input, curr);
        }
        input_buffer_push(input, '\0');
        if (!strcmp(input->buffer, "exit")) break;

        cmd = command_parse_new(input->buffer);
        if (cmd) {
            int status = command_handle(cmd);
            if (status == -1)
                printf("Invalid set of arguments was given to the command '%s'\n", cmd->name);
            shell_history_prepend(history, cmd);
        }

        input_free(input);
    }
    command_free(cmd);
    shell_history_free(history);

    return 0;
}