#include "../include/common.h"
#include "../include/input_buffer.h"


/*
 *
 */
input_t*
input_new()
{
    input_t* input = (input_t*)sh_malloc(sizeof(input_t));
    input->capacity = INPUT_BUFFER_CAP_INIT;
    input->size = 0;
    input->buffer = (char*)sh_malloc(input->capacity);

    return input;
}

/*
 *
 */
void
input_buffer_push(input_t* input, char ch)
{
    if (input->size >= input->capacity)
        input_buffer_reallocate(input, input->capacity * INPUT_BUFFER_CAP_EXP_VAL);

    input->buffer[input->size++] = ch;
}

/*
 *
 */
void input_buffer_push_at(input_t* input, char ch, unsigned pos)
{
    if (pos > input->size) return;

    // in order not to make redudant memmove
    if (pos == input->size - 1)
        input_buffer_push(input, ch);
    else {
        if (input->size >= input->capacity)
            input_buffer_reallocate(input, input->capacity * INPUT_BUFFER_CAP_EXP_VAL);
        
        memmove(
            &(input->buffer[pos + 1]),
            &(input->buffer[pos]),
            input->size - pos
        );

        input->buffer[pos] = ch;
        input->size++;
    }
}

/*
 *
 */
void input_buffer_push_string(input_t* input, char* string, unsigned size)
{
    for (int i = 0; i < size; i++)
        input_buffer_push(input, string[i]);
}

/*
 *
 */
void input_buffer_pop(input_t* input)
{
    if (input->size > 0) {
        input_buffer_pop_at(input, input->size - 1);
    }
}

/*
 *
 */
void input_buffer_pop_at(input_t* input, unsigned pos)
{
    if (input->size > 0 && pos >= 0 && pos < input->size) {
        size_t mem_block = input->size - pos;
        memmove(
            &(input->buffer[pos]),
            &(input->buffer[pos + 1]),
            mem_block
        );
        input->size--;
    }
}

/*
 *
 */
void input_buffer_pop_last_n(input_t* input, unsigned bytes)
{
    // TODO:: with one memmove
    for (int i = 0; i < bytes; i++)
        input_buffer_pop(input);
}

/*
 *
 */
void
input_buffer_reallocate(input_t* input, unsigned bytes)
{
    input->buffer = realloc((void*)input->buffer, bytes);
    input->capacity = bytes;
}

/*
 *
 */
void
input_free(input_t* input)
{
    input->size = 0;
    free(input->buffer);
    free(input);
}