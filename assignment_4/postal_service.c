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

#include <stdbool.h>

#include "mailbox.h"


//
unsigned long **sys_call_table;

// the old syscalls
asmlinkage long  (*old_call1)  (void);
asmlinkage long  (*old_call2)  (void);
asmlinkage long  (*old_call3)  (void);




asmlinkage long new_sys_cs3013_syscall1(void)
{
	printk(KERN_INFO "\"'Hello world?!' More like 'Goodbye, world!' EXTERMINATE!\" -- Dalek");
	return 0;
}	// asmlinkage long new_sys_cs3013_syscall1(void)



asmlinkage long new_fopen(const char* file, int flags, int mode)
{
	int uid = getuid_call();
	if (uid >= 1000)
	{
		printk(KERN_INFO "file: \"%s\" opened by user with id: %i\n", file, uid);
	}

	return old_sys_open(file, flags, mode);
}


asmlinkage long new_fclose(const char* file, int flags, int mode)
{
	int uid = getuid_call();
	if (uid >= 1000)
	{
		printk(KERN_INFO "file: \"%s\" closed by user with id: %i\n", file, uid);
	}

	return old_sys_close(file, flags, mode);
	
}




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






static int __init interceptor_start(void)
{
	/* Find the system call table */
	if(!(sys_call_table = find_sys_call_table()))
	{
		/* Well, that didn't work.
		Cancel the module loading step. */
		return -1;
	}


	/* Store a copy of all the existing functions */
	old_sys_open  = (void *)sys_call_table[__NR_open];
	old_sys_close = (void *)sys_call_table[__NR_close];
	getuid_call =   (void *)sys_call_table[__NR_getuid];

	/* Replace the existing system calls */
	disable_page_protection();

	sys_call_table[__NR_open]  = (unsigned long *)new_fopen;
	sys_call_table[__NR_close] = (unsigned long *)new_fclose;

	enable_page_protection();

	/* And indicate the load was successful */
	printk(KERN_INFO "Loaded interceptor!\n");

	return 0;
}	// static int __init interceptor_start(void)






static void __exit interceptor_end(void)
{
	/* If we don't know what the syscall table is, don't bother. */
	if(!sys_call_table)
		return;

	/* Revert all system calls to what they were before we began. */
	disable_page_protection();
	sys_call_table[__NR_open]  = (unsigned long *) old_sys_open;
	sys_call_table[__NR_close] = (unsigned long *) old_sys_close;
	enable_page_protection();

	printk(KERN_INFO "Unloaded interceptor!\n");
}	// static void __exit interceptor_end(void)






MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);

