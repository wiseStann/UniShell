#include "../include/interact.h"


//
void handle_keyboard_input(input_t* input, int* cursor_pos_in_command, int key)
{
    static unsigned previous_watched_command_len;
	switch (key)
	{
    case ARROW_UP:
        previous_watched_command_len = handle_arrow_up(input, cursor_pos_in_command, previous_watched_command_len);
        break;
    case ARROW_DOWN:
        previous_watched_command_len = handle_arrow_down(input, cursor_pos_in_command, previous_watched_command_len);
        break;
    case ARROW_LEFT:
        handle_arrow_left(input, cursor_pos_in_command);
        break;
    case ARROW_RIGHT:
        handle_arrow_right(input, cursor_pos_in_command);
        break;
    case BACKSPACE:
        handle_backspace(input, cursor_pos_in_command);
        break;
    default:
        if (*cursor_pos_in_command < input->size) {
            write(1, &key, 1);
            for (int i = *cursor_pos_in_command; i < input->size; i++) {
                write(1, &input->buffer[i], 1);
            }

            CURSOR_MOVE_LEFT(input->size - *cursor_pos_in_command);

            input_buffer_push_at(input, key, *cursor_pos_in_command);
            (*cursor_pos_in_command)++;
        }
        else {
            write(1, &key, 1);
            input_buffer_push(input, key);
            (*cursor_pos_in_command)++;
        }
	}
}

//
unsigned handle_arrow_up(input_t* input, int* cursor_pos_in_command, unsigned previous_watched_command_len)
{
    char esc_seq[ESC_SEQUENCE_MAX_LEN];
    if (!history->curr_watching) history->curr_watching = history->head;
    else if (history->curr_watching->next) history->curr_watching = history->curr_watching->next;

    if (history->curr_watching) {
        char tmp[history->curr_watching->command->length + ESC_SEQUENCE_MAX_LEN];
        strcpy(tmp, history->curr_watching->command->content);
        previous_watched_command_len = input->size;

        if (previous_watched_command_len > 0) {
            sprintf(esc_seq, "\033[%dD", previous_watched_command_len);
            char_array_prepend(tmp, esc_seq);
        }

        if (previous_watched_command_len > 0)
            write(1, tmp, history->curr_watching->command->length + strlen(esc_seq));
        else
            write(1, tmp, history->curr_watching->command->length);
        *cursor_pos_in_command += history->curr_watching->command->length;

        input_buffer_pop_last_n(input, previous_watched_command_len);
        input_buffer_push_string(input, history->curr_watching->command->content, history->curr_watching->command->length);

        if (history->curr_watching->command->length < previous_watched_command_len) {
            for (int i = history->curr_watching->command->length; i < previous_watched_command_len; i++) {
                char space = SPACE_SYMBOL;
                write(1, &space, 1);
            }
            CURSOR_MOVE_LEFT(previous_watched_command_len - history->curr_watching->command->length);
        }
    }
    return previous_watched_command_len;
}

//
unsigned handle_arrow_down(input_t* input, int* cursor_pos_in_command, unsigned previous_watched_command_len)
{
    char esc_seq[ESC_SEQUENCE_MAX_LEN];
    if (history->curr_watching) history->curr_watching = history->curr_watching->prev;
    previous_watched_command_len = input->size;

    if (history->curr_watching) {
        char tmp[history->curr_watching->command->length + ESC_SEQUENCE_MAX_LEN];
        strcpy(tmp, history->curr_watching->command->content);

        if (previous_watched_command_len > 0) {
            sprintf(esc_seq, "\033[%dD", previous_watched_command_len);
            char_array_prepend(tmp, esc_seq);
        }
        write(1, tmp, history->curr_watching->command->length + strlen(esc_seq));
        input_buffer_pop_last_n(input, previous_watched_command_len);
        input_buffer_push_string(input, history->curr_watching->command->content, history->curr_watching->command->length);

        if (history->curr_watching->command->length < previous_watched_command_len) {
            for (int i = history->curr_watching->command->length; i < previous_watched_command_len; i++) {
                char space = SPACE_SYMBOL;
                write(1, &space, 1);
            }
            CURSOR_MOVE_LEFT(previous_watched_command_len - history->curr_watching->command->length);
        }
    }
    else {
        for (int i = 0; i < previous_watched_command_len; i++) {
            write(1, "\033[1D \033[1D", strlen("\033[1D \033[1D"));
        }
        input_buffer_pop_last_n(input, previous_watched_command_len);
        *cursor_pos_in_command = 0;
    }
}

//
void handle_arrow_left(input_t* input, int* cursor_pos_in_command)
{
    if (*cursor_pos_in_command > 0) {
        CURSOR_MOVE_LEFT(1);
        (*cursor_pos_in_command)--;
    }
}

//
void handle_arrow_right(input_t* input, int* cursor_pos_in_command)
{
    if (*cursor_pos_in_command < input->size) {
        CURSOR_MOVE_RIGHT(1);
        (*cursor_pos_in_command)++;
    }
}

//
void handle_backspace(input_t* input, int* cursor_pos_in_command)
{
    if (input->size > 0) {
        input_buffer_pop_at(input, --(*cursor_pos_in_command));
        write(1, "\033[1D \033[1D", strlen("\033[1D \033[1D"));

        if (*cursor_pos_in_command != input->size) {
            for (int i = *cursor_pos_in_command; i < input->size; i++) {
                write(1, &input->buffer[i], 1);
            }

            write(1, " \033[1D", strlen(" \033[1D"));
            CURSOR_MOVE_LEFT(input->size - *cursor_pos_in_command);
        }
    }
}
