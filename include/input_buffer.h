#ifndef SHELL_MAIN_H
#define SHELL_MAIN_H

#define INPUT_BUFFER_CAP_INIT 16
#define INPUT_BUFFER_CAP_EXP_VAL 1.5


// input buffer structure
typedef struct input_buff {
    char* buffer;
    unsigned size;
    unsigned capacity;
} input_t;


//
input_t* input_new();

//
void input_buffer_push(input_t*, char);

//
void input_buffer_push_at(input_t*, char, unsigned);

//
void input_buffer_push_string(input_t*, char*, unsigned);

//
void input_buffer_pop(input_t*);

//
void input_buffer_pop_last_n(input_t*, unsigned);

// 
void input_buffer_reallocate(input_t*, unsigned);

//
void input_free(input_t*);


#endif // !SHELL_MAIN_H