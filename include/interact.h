#ifndef SHELL_INTERACT_H
#define SHELL_INTERACT_H

#include "common.h"
#include "utils.h"
#include "input_buffer.h"


void handle_keyboard_input(input_t*, int*, int);

unsigned handle_arrow_up(input_t*, int*, unsigned);

unsigned handle_arrow_down(input_t*, int*, unsigned);

void handle_arrow_left(input_t*, int*);

void handle_arrow_right(input_t*, int*);

void handle_backspace(input_t*, int*);


#endif // !SHELL_INTERACT_H