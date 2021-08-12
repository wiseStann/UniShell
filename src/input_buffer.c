#include "../include/common.h"
#include "../include/input_buffer.h"


/*
 *
 */
input_t*
input_new()
{
    input_t* input = (input_t*)malloc(sizeof(input_t));
    input->capacity = INPUT_BUFFER_CAP_INIT;
    input->size = 0;
    input->buffer = (char*)malloc(input->capacity);

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
    free(input->buffer);
    free(input);
}