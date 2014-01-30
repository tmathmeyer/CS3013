/*
 * sys_open_lkm.c
 *
 *  Undated on: Jan 26, 2014
 *      Author: Craig Shue
 *      Updated: Hugh C. Lauer
 *		Updated: Ted Meyer (tmathmeyer@gmail.com)
 */

// We need to define __KERNEL__ and MODULE to be in Kernel space
// If they are defined, undefined them and define them again:
#undef __KERNEL__
#undef MODULE

#define __KERNEL__ 
#define MODULE

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/cred.h>

//pointer to the system call table
unsigned long **sys_call_table;

// the old syscalls
asmlinkage long  (*old_sys_open)  (void);
asmlinkage long  (*old_sys_close) (void);
asmlinkage int   (*getuid_call)   (void);




asmlinkage long new_sys_cs3013_syscall1(void)
{
	printk(KERN_INFO "\"'Hello world?!' More like 'Goodbye, world!' EXTERMINATE!\" -- Dalek");
	return 0;
}	// asmlinkage long new_sys_cs3013_syscall1(void)



asmlinkage long new_fopen(void)
{



	return 0;
}


asmlinkage long new_fclose(void)
{



	return 0;
}




static unsigned long **find_sys_call_table(void)
{
	unsigned long int offset = PAGE_OFFSET;
	unsigned long **sct      = 0;

	while (offset < ULLONG_MAX)
	{
		sct = (unsigned long **)offset;

		if (sct[__NR_close] == (unsigned long *) sys_close)
		{
			printk(KERN_INFO "Interceptor: Found syscall table at address: 0x%02lX", (unsigned long) sct);
			return sct;
		}

		offset += sizeof(void *);
	}

	return NULL;
}	// static unsigned long **find_sys_call_table(void)






static void disable_page_protection(void)
{
	/*
	Control Register 0 (cr0) governs how the CPU operates.

	Bit #16, if set, prevents the CPU from writing to memory marked as
	read only. Well, our system call table meets that description.
	But, we can simply turn off this bit in cr0 to allow us to make
	changes. We read in the current value of the register (32 or 64
	bits wide), and AND that with a value where all bits are 0 except
	the 16th bit (using a negation operation), causing the write_cr0
	value to have the 16th bit cleared (with all other bits staying
	the same. We will thus be able to write to the protected memory.

	It's good to be the kernel!
	*/

	write_cr0 (read_cr0 () & (~ 0x10000));

}	//static void disable_page_protection(void)






static void enable_page_protection(void)
{
	/*
	See the above description for cr0. Here, we use an OR to set the
	16th bit to re-enable write protection on the CPU.
	*/

	write_cr0 (read_cr0 () | 0x10000);

}	// static void enable_page_protection(void)






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

	//sys_call_table[__NR_open]  = (unsigned long *)new_fopen;
	//sys_call_table[__NR_close] = (unsigned long *)new_fclose;

	enable_page_protection();


	uid_t uid = getuid_call();
	printk(KERN_INFO "you are user_id #%i", uid);


	/* And indicate the load was successful */
	printk(KERN_INFO "Loaded interceptor!");

	return 0;
}	// static int __init interceptor_start(void)






static void __exit interceptor_end(void)
{
	/* If we don't know what the syscall table is, don't bother. */
	if(!sys_call_table)
		return;

	/* Revert all system calls to what they were before we began. */
	disable_page_protection();
	//sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)ref_sys_cs3013_syscall1;
	enable_page_protection();

	printk(KERN_INFO "Unloaded interceptor!");
}	// static void __exit interceptor_end(void)






MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);

