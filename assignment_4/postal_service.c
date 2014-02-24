/*
 * postal_service.c
 *   manages the mailboxes (hahaha)
 *
 *  Author: Ted Meyer
 *  Date:   2014-02-17
 *  License: GPLv2
 */


//definitions
#undef __KERNEL__
#undef MODULE

#define __KERNEL__ 
#define MODULE





//includes
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/cred.h>
#include <linux/types.h>
#include <linux/slab.h>

#include <stdbool.h>

#include "mailbox.h"
#include "mailstructs.h"
#include "hashmap.h"


//
unsigned long **sys_call_table;

// the old syscalls
asmlinkage long  (*old_call1)  (void);
asmlinkage long  (*old_call2)  (void);
asmlinkage long  (*old_call3)  (void);




asmlinkage long send_message(pid_t recip, void* msg, int len, bool block)
{
	printk(KERN_INFO "send mail");
	return 2;
}


asmlinkage long receive(pid_t* sender, void* msg, int* len, bool block)
{
    printk(KERN_INFO "get mail");
    return 2;
}


asmlinkage long manage_mail(bool stop, int* vol)
{
    printk(KERN_INFO "manage");
    return 2;
}




/*
 *==============================Syscall Table functions========================================
 */
static unsigned long **find_sys_call_table(void)
{
	unsigned long int offset;

	for (offset=PAGE_OFFSET; offset<ULLONG_MAX; offset+=sizeof(void*))
	{
		if ( ((unsigned long **)offset)[__NR_close] == (unsigned long *)sys_close )
		{
			return (unsigned long **)offset;
		}
	}

	return NULL;
}


static void disable_page_protection(void)
{
	write_cr0 (read_cr0 () & (~ 0x10000));
}

static void enable_page_protection(void)
{
	write_cr0 (read_cr0 () | 0x10000);
}

/*
 *=============================End   Syscall Table Functions===================
 *=============================Begin module Functions==========================
 */


struct kmem_cache* mailboxes;
struct kmem_cache* messages;
int in_operation = 0;



static int __init module_start(void)
{
    //initialize the mailbox slab (processes)
    mailboxes = kmem_cache_create("mailboxes", sizeof(mailbox), 0, 0, NULL);

    //initialize the number of messages to 128 (messages)
    messages = kmem_cache_create("messages",   sizeof(message), 0, 0, NULL);
    
    //initialize the mailbox hashmap
    map_init();

    //swap the functions
    interceptor_start();
    in_operation = 1;
    
    return 0;
}

static void __exit module_end(void)
{
    //swap functions back to saved state
    interceptor_end();

    //free all structs

    //close the caches (lock access)
    
    //some sort of lock thing here
    kmem_cache_destroy(mailboxes);
    kmem_cache_destroy(messages);
    in_operation = 0;
    //unlock errythin
}










static int interceptor_start(void)
{
	if(!(sys_call_table = find_sys_call_table()))
	{
		return -1;
	}

	old_call1 = (void *)sys_call_table[__NR_cs3013_syscall1];
    old_call2 = (void *)sys_call_table[__NR_cs3013_syscall2];
    old_call3 = (void *)sys_call_table[__NR_cs3013_syscall3];

	disable_page_protection();
    //{
	    sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)send_message;
	    sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)receive;
        sys_call_table[__NR_cs3013_syscall3] = (unsigned long *)manage_mail;
    //}
	enable_page_protection();

	printk(KERN_INFO "Postal Service has set up shop!");

	return 0;
}





static void interceptor_end(void)
{
	if(!sys_call_table)
		return;

	disable_page_protection();
    //{
        sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)old_call1;
        sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)old_call2;
        sys_call_table[__NR_cs3013_syscall3] = (unsigned long *)old_call3;
    //}
    enable_page_protection();

	printk(KERN_INFO "Unloaded interceptor!\n");
}




MODULE_LICENSE("GPL");
module_init(module_start);
module_exit(module_end);

