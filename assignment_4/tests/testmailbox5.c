/* Alfredo Porras
 * July 12th, 2011
 * CS 3013
 * Project 4 - test program 5
 * Tests if mailbox can be deleted while other processes
 * are holding a pointer to it.
 */

#include "mailbox.h"
#include <stdio.h>
#include <unistd.h>

#define CHILD_NUM 1

int main()
{
    int childCounter;
    int msgs;

    // fork enough children so that they can all send a message
    for(childCounter = 0; childCounter < CHILD_NUM; childCounter++)
    {
        int childPID = fork();
        
        if(childPID == 0)
        {
            pid_t sender;
            void *msg[128];
            int len;
            RcvMsg(&sender,msg,&len,true);
            printf("PMessage: %s\n", (char *)msg);
            char myMesg[] = "I am your child";


            if(SendMsg(sender, myMesg, 16, true))
            {
                printf("Child send failed.\n");
            }
            
            return 0;
        }
        else
        {
            sleep(2);
            char mesg[] = "I am your father";
            if (SendMsg(childPID, mesg, 17, false)){
                printf("Send failed\n");
            }
        }
    }
    
    // the parent sleeps for a little time
    // waiting for some of it's children to get a pointer
    // to its mailbox
    // before trying to kill its own process.
    usleep(100000);
    
    return 0;
}
