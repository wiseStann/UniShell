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
void input_buffer_push_at(input_t* input, char ch, unsigned int pos)
{
    // in order not to make redudant memmove
    if (pos == input->buffer[input->size])
        input_buffer_push(input, ch);
    else {
        if (input->size >= input->capacity)
            input_buffer_reallocate(input, input->capacity * INPUT_BUFFER_CAP_EXP_VAL);
        
        size_t mem_block = (input->size - pos) * sizeof(char*);
        memmove(
            &(input->buffer[pos + 1]),
            &(input->buffer[pos]),
            mem_block
        );

        input->buffer[pos] = ch;
        input->size++;
    }
}

/*
 *
 */
void input_buffer_push_string(input_t* input, char* string, unsigned int size)
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
        memmove (
            &(input->buffer[input->size - 1]),
            &(input->buffer[input->size]),
            sizeof(char*)
        );
        input->size--;
    }
}

/*
 *
 */
void input_buffer_pop_last_n(input_t* input, unsigned bytes)
{
    for (int i = 0; i < bytes; i++)
        input_buffer_pop(input);
}

/*
 *
 */
void
input_buffer_reallocate(input_t* input, unsigned int bytes)
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