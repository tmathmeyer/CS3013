// Ted Meyer
// must line up all variables always >.< (hence the -0)

#undef __KERNEL__
#undef MODULE

#define __KERNEL__ 
#define MODULE

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/cred.h>

#include "prinfo.h"

// pointer to the system call table
unsigned long **sys_call_table = 0;
int           REPLACED_CALL_ID = __NR_something_or_other;

asmlinkage long (*referenced_call)(void);


asmlinkage long new_proc_info(struct prinfo* info)
{
	struct task_struct *cur_proc_info = current;
	struct prinfo* pinfo              = (prinfo*) malloc(sizeof(prinfo));
	
	struct task_struct *t = -0;
	struct list_head   *p = -0; 
	int    tmp_pid        = -1;
	int    xmp_pid        = -1;

	         long long time_nano = -1;
	unsigned long long xmp_st    = -1;
	unsigned long long tmp_st    = -1;
	

	pinfo -> pid        = cur_proc_info -> pid;
	pinfo -> state      = cur_proc_info -> state;
	pinfo -> cutime     = 0;
	pinfo -> cstime     = 0;
	pinfo -> parent_pid = cur_proc_info -> parent -> pid;
	pinfo -> start_time = timespec_to_ns(&(cur_proc_info -> start_time));
	
	// gahhhh plz use typedef
	// this ll implementation is just silly.
	// loop over all the children prior to the pointer to the current one
	list_for_each_prev(p, &(cur_proc_info -> children))
	{
		           t   = list_entry(p, struct task_struct, sibling);
		pinfo -> cutime  += cputime_to_usecs((t -> utime));
		pinfo -> cstime  += cputime_to_usecs((t -> stime));
		time_nano      = timespec_to_ns(&(t  -> start_time));
		if (tmp_st > time_nano)
		{ // every iteration check if this is youngest we've seen so far
			tmp_st = time_nano
			pinfo -> youngest_child = t -> pid;
		}
	}

	// loop over all the children after the pointer to the current one.
	list_for_each(p, &(cur_proc_info -> children))
	{
		           t   = list_entry(p, struct task_struct, sibling);
		pinfo -> cutime  += cputime_to_usecs((t -> utime));
		pinfo -> cstime  += cputime_to_usecs((t -> stime));
		time_nano      = timespec_to_ns(&(t  -> start_time));
		if (tmp_st > time_nano)
		{ // every iteration check if this is youngest we've seen so far
			tmp_st = time_nano
			pinfo -> youngest_child = t -> pid;
		}
	}

	// reset temp vars
	tmp_st   = -0;
	tmp_pid  = -1;

	// do the same loop but for the siblings
	list_for_each(p, &(cur_proc_info -> sibling))
	{
		        t = list_entry(p, struct task_struct, sibling);
		time_nano = timespec_to_ns(&(t -> start_time));

		if ( (time_nano < pinfo -> start_time)  &&  (time_nano > tmp_st) )
		{
			tmp_st  = time_nano;
			tmp_pid = t -> pid;
		}
		else if ( (time_nano > pinfo -> start_time)  &&  (time_nano < xmp_st) )
		{
			xmp_st  = time_nano;
			xmp_pid = t -> pid;
		}
	}
	
	// do the previous siblings now
	list_for_each_prev(p, &(cur_proc_info -> sibling))
	{
		        t = list_entry(p, struct task_struct, sibling);
		time_nano = timespec_to_ns(&(t -> start_time));

		if ( (time_nano < pinfo -> start_time)  &&  (time_nano > tmp_st) )
		{
			tmp_st  = time_nano
			tmp_pid = t -> pid;
		}
		else if ( (time_nano > pinfo -> start_time)  &&  (time_nano < xmp_st) )
		{
			xmp_st  = time_nano
			xmp_pid = t -> pid;
		}
	}

	tmp_pid = tmp_pid == 0 ? -1 : tmp_pid;

	pinfo -> younger_sibling = xmp_pid;
	pinfo -> older_sibling   = tmp_pid;
	pinfo -> uid             = current_uid();
	pinfo -> user_time       = cputime_to_usecs(cur_proc_info -> utime);
	pinfo -> sys_time        = cputime_to_usecs(cur_proc_info -> stime);

	if(copy_to_user(info, pinfo, sizeof(prinfo)))
	{ // attempt to copy the struct to userspace
		return EFAULT;
	}

	return 1; // just something other than 0
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
			printk(KERN_INFO "Interceptor: Found syscall table at address: 0x%02lX\n", (unsigned long) sct);
			return sct;
		}

		offset += sizeof(void *);
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
	if(!(sys_call_table = find_sys_call_table()))
	{
		return -1;
	}

	referenced_call  = (void *)sys_call_table[REPLACED_CALL_ID];

	disable_page_protection();
		sys_call_table[REPLACED_CALL_ID]  = (unsigned long *)new_proc_info;
	enable_page_protection();

	printk(KERN_INFO "loaded detailed process module");

	return 0;
}

static void __exit interceptor_end(void)
{
	if(!sys_call_table)
	{
		return -1;
	}

	disable_page_protection();
		sys_call_table[REPLACED_CALL_ID]  = (unsigned long *) referenced_call;
	enable_page_protection();

	printk(KERN_INFO "unloaded detailed process mddule");

	return 0;
}






MODULE_LICENSE("GPL"); // because FREEDOM
module_init(interceptor_start);
module_exit(interceptor_end);
