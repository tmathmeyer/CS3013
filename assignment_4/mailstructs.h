/* Ted Meyer
 * mailstructs.h
 * 2014-02-23
 * License: GPLv2
 */

#ifndef __MAILSTRUCTS__
#define __MAILSTRUCTS__


#include "mailbox.h"

// functions
static int  interceptor_start(void);
static void interceptor_end  (void);


// struct for messages (ll)
typedef struct message {
        struct message *next;
        char   data[MAX_MSG_SIZE];
        pid_t  sender;
        pid_t  dest;
} message;

// struct for mailbox (container)
typedef struct mailbox {
        pid_t owner;
        int msg_count;
        message* contents;
} mailbox;

#endif
