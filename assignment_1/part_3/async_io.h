//Ted Meyer
//this didnt get used, but im keeping it here for reference

#ifndef ASYNC_IO_H
#define ASYNC_IO_H

#include <pthread.h>

typedef struct io_buffer {
    char* buffr;
    int r_pos;
    int w_pos;
    struct io_buffer* next;
} io_buffer;

typedef struct _thread_data {
    char* terminator;
} _thread_data;

io_buffer* _head;
io_buffer* _tail;

//read a character... ASYNCHRONOUSLY
//returns -1 rather than blocking
char getchar_as();

//start the buffering process
int init(int* pipes, pthread_t* thread);

//reads char from sysin and buffers it
int _insert(char c);


#endif
