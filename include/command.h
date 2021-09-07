#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H

#define COMMAND_NAME_MAX_LEN 256
#define COMMAND_ARGUMENT_MAX_LEN 512


// command argument structure
typedef struct arg {
    char name[COMMAND_ARGUMENT_MAX_LEN];
    unsigned size;
    unsigned idx_number;
} argument_t;

// command structure
typedef struct cmd {
    char* name;
    char* content;
    unsigned length;
    argument_t** arguments;
    unsigned args_num;
    unsigned table_index;
    int is_valid;
} command_t;


// creates a new command argument
argument_t* command_argument_new(unsigned int);

// creates a new command struct based on a given command
command_t* command_new(char*, int);

// returns the length of the command
unsigned command_length(const char*, int);

// trims the command
void command_trim(char*);

// shows a command information
void command_info(command_t*);

// frees a command struct
void command_free(command_t*);

// converts command argument_t array to string array 
char** command_args_to_string_array(command_t*);

//
extern void usage(command_t*);


#endif // !SHELL_COMMAND_H