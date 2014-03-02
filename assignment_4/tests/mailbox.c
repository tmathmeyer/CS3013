/*
 * Adapted from CS-502 Project #3, Fall 2006
 *	originally submitted by Cliff Lindsay
 * Modified for CS-502, Summer 2011 by Alfredo Porras
 * Adapted for CS-502 at Cisco Systems, Fall 2011
 * 
 * Modified Ted Meyer for CS3013
 *   2014-02-26
 *
 * License: GPLv2
 */

#include "mailbox.h"
#include <sys/syscall.h>
#include <unistd.h>

#define SEND 349
#define RECV 350
#define MANG 351

/*
 * Functions for sending messages
 * 
 */
int SendMsg(pid_t dest, void *msg, int len, bool block)
{
    return syscall(__NR_mailbox_send, dest, msg, len, block);
}

/*
 * function for recieving messages
 */
int RcvMsg(pid_t *sender, void *msg, int *len, bool block)
{
    return syscall(__NR_mailbox_rcv, sender, msg, len, block);
}

/*
 * functions for maintaining mailboxes
 * 
 */
int ManageMailbox(bool stop, int *count)
{
    return syscall(MANG, stop, count);
}

