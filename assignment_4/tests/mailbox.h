/*
 * Adapted from CS-502 Project #3, Fall 2006
 *	 originally submitted by Cliff Lindsay
 * 
 * Modified for CS-3013, C-term 2015
 * Modified by Ted Meyer on 2014-02-17
 *
 * License: GPLv2
 */

#ifndef __MAILBOX__
#define __MAILBOX__

#include <stdbool.h>

#ifndef __KERNEL__
#include <sys/types.h>
#else
#include <linux/types.h>
#endif

#define NO_BLOCK 0
#define BLOCK   1
#define MAX_MSG_SIZE 128

/*
 * Functions for msgs 
 */
int SendMsg(pid_t dest, void *msg, int len, bool block);
int RcvMsg(pid_t *sender, void *msg, int *len, bool block);

/*
 * functions for maintaining mailboxes
 */
int ManageMailbox(bool stop, int *count);

/*
 * error codes pertaining to mailboxes 
 */
#define MAILBOX_FULL		1001
#define MAILBOX_EMPTY		1002
#define MAILBOX_STOPPED		1003
#define MAILBOX_INVALID		1004
#define MSG_LENGTH_ERROR	1005
#define MSG_ARG_ERROR		1006
#define MAILBOX_ERROR		1007

#endif
