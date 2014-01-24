//Ted Meyer

#include "async_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <unistd.h>

void* read_loop(void* data);

char getchar_as()
{
    if (_head -> w_pos == _head -> r_pos)
    { // the read and write heads are at the same position
        return -2;
    }
    else
    {
        if (_head -> r_pos == 128)
        { //we're out of the buffer chunk. go to the next one
            io_buffer* next = _head -> next;
            free(_head -> buffr);
            free(_head);
            _head = next;
        }
        
        if (_head -> r_pos == _head -> w_pos)
        {
            return -2;
        }
        else
        {
            char* get  = _head -> buffr;
                  get += _head -> r_pos;
            char res = *(get);
            _head -> r_pos ++;
            return res;
        }
    }
}


int init(int* pipes, pthread_t* thread)
{
    _head = (io_buffer*)(malloc(sizeof(io_buffer)));
    _head -> buffr = (char*)(malloc(128));
    _head -> r_pos = 0;
    _head -> w_pos = 0;
    _head -> next  = 0;
    _tail = _head;


    int r;
    pipe(pipes);

    if (r = pthread_create(thread, NULL, read_loop, &pipes[0])) {
        printf("Error: %d\n", r);
        return 1;
    }

}

int _insert(char c)
{
    if (_tail -> w_pos == 128)
    { // gotta create a new place
        _tail -> w_pos = -1;
        io_buffer* next = (io_buffer*)(malloc(sizeof(io_buffer)));
        next -> buffr = (char*)(malloc(128));
        next -> r_pos = 0;
        next -> w_pos = 1;
        next -> next  = 0;
        
        _tail -> next = next;
        _tail = next;
        *(_tail -> buffr) = c;
        
    }
    else
    {
        *((_tail -> buffr) + (_tail -> w_pos))
            = c;
        _tail -> w_pos++;
    }
}

void* read_loop(void* p)
{
    int readpipe = *(int *)p;
    fd_set rfds;

    char c;

    for (;;)
    {
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        FD_SET(readpipe, &rfds);
        while (select(readpipe + 1, &rfds, NULL, NULL, NULL) == 0);
        if (FD_ISSET(readpipe, &rfds))
        {
            close(readpipe);
            break;
        }
        if (FD_ISSET(STDIN_FILENO, &rfds))
        {
            if (read(STDIN_FILENO, &c, sizeof(c)) > 0)
            {
                _insert(c);
            }
        }
    }

    printf("Thread terminating\n");

    pthread_exit(NULL);
}