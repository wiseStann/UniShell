#include "include/common.h"
#include "include/parser.h"
#include "include/input_buffer.h"


int main(int argc, char** argv)
{
    input_t* input = NULL;
    command_t* cmd = NULL;
    char prev, curr;

    while (TRUE) {
        input = input_new();
        printf("shell>$ ");
        while ((curr = getchar()) != ENTER_CODE) {
            if (curr != BACK_SLASH_SYMBOL)
                input_buffer_push(input, curr);
            prev = curr;
        }
        input_buffer_push(input, '\0');
        if (!strcmp(input->buffer, "exit")) break;

        cmd = command_parse_new(input->buffer);
        if (cmd) {
            printf("Command name: %s\n", cmd->name);
            printf("Arguments:\n");
            for (int i = 0; i < cmd->args_num; i++) {
                printf("Name: '%s', size: %d, index number: %d\n", cmd->arguments[i]->name, cmd->arguments[i]->size, cmd->arguments[i]->idx_number);
            }
            printf("\n");

            int status = command_handle(cmd);
            if (!status)
                printf("Invalid set of arguments was given to the command '%s'\n", cmd->name);
        }

        input_free(input);
    }
    command_free(cmd);

    return 0;
}