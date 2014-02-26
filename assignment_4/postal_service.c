/*
 * postal_service.c
 *   manages the mailboxes (hahaha)
 *
 *  Author: Ted Meyer
 *  Date:   2014-02-17
 *  License: GPLv2
 */


/*
 *=============================End Header=======================================
 *=============================Begin Definitions================================
 */


#undef __KERNEL__
#undef MODULE

#define __KERNEL__ 
#define MODULE

#define MAP_SIZE 16
#define MAILBOX_SIZE 128

/*
 *=============================End Defenitions==================================
 *=============================Begin Includes===================================
 */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/cred.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <stdbool.h>
#include "mailbox.h"


/*
 *=============================End Includes=====================================
 *=============================Begin Structs====================================
 */


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
        char unblocked;
} mailbox;

// struct for hashmap
typedef struct map_elem {
        struct map_elem* next;
        pid_t            id;
        mailbox*         box;
} map_elem;


/*
 *=============================End Structs======================================
 *=============================Begin Function Prototypes========================
 */


// hashmap calls
int map_init(void);
int map_put(pid_t, mailbox* box);
int map_rem(pid_t id);
int map_stop(void);
mailbox* map_get(pid_t id);

// interceptor calls
static int  interceptor_start(void);
static void interceptor_end  (void);

// the old syscalls
asmlinkage long  (*old_call1)  (void);
asmlinkage long  (*old_call2)  (void);
asmlinkage long  (*old_call3)  (void);


/*
 *=============================End Function Prototypes==========================
 *=============================Begin Global Vars================================
 */


// the syscall table
unsigned long **sys_call_table;

// the map element pointers
map_elem** map;

struct kmem_cache* mailboxes;
struct kmem_cache* messages;
struct kmem_cache* hashmap;

int in_operation = 0;


/*
 *=============================End Global Vars==================================
 *=============================Begin Hashmap Functions==========================
 */


int map_init()
{
    int i;
    hashmap = kmem_cache_create("hashmap_pid_to_mailbox", sizeof(map_elem), 0, 0, NULL);
    map = (map_elem**) kmalloc(MAP_SIZE * sizeof(map_elem*), GFP_KERNEL);
    for(i=0; i<MAP_SIZE; i++)
    {
        *(map + i) = 0;
    }
    return 0;
}

int map_put(pid_t id, mailbox* box)
{
    map_elem* list = *(map + id%MAP_SIZE);
    map_elem* n =  kmem_cache_alloc(hashmap, GFP_KERNEL);
              n -> id   = id;
              n -> box  = box;
              n -> next = 0;
    if (list == 0)
    {
        *(map + id%MAP_SIZE) = n;
    }

    while(list != 0)
    {
        if (list -> id == id)
        {
            list -> box = box;
            return 0;
        }
        if (list -> next == 0)
        {
            list -> next = n;
        }
        list = list -> next;
    }
    return 0;
}

mailbox* map_get(pid_t id)
{
    map_elem* list = *(map + id%MAP_SIZE);
    while(list != 0)
    {
        if (list -> id == id)
        {
            return list -> box;
        }
        list = list -> next;
    }
    return 0;
}

int map_rem(pid_t id)
{
    map_elem* list = *(map + id%MAP_SIZE);
    if (list == 0)
    {
        return 0;
    }
    
    while(list -> next != 0)
    {
        if (list -> next -> id == id)
        {
            map_elem* del = list -> next;
            list -> next = list -> next -> next;
            kmem_cache_free(hashmap, del);
            return 0;
        }
    }
    return 0;
}

int map_stop()
{
    int i = 0;
    while(i < MAP_SIZE)
    {
        map_elem* front = *(map + i);
        map_elem* kill = 0;
        while(front != 0)
        {
            kill = front;
            front = front -> next;
            kmem_cache_free(hashmap, kill);
        }
        i++;
    }
    kmem_cache_destroy(hashmap);
    return 0;
}


/*
 *=============================End Hashmap Functions============================
 *=============================Begin New System Calls===========================
 */


asmlinkage long send_message(pid_t recip, void* msg, int len, bool block)
{
    mailbox* recipient;
    struct message* newmsg;
    int j;

    if (len < 0 || len > MAX_MSG_SIZE)
    {
        return MSG_LENGTH_ERROR;
    }
    if (recip < 0 || msg < 0 || !(block==BLOCK || BLOCK==NO_BLOCK))
    {
        return MSG_ARG_ERROR;
    }

    do
    {
        //lock the mutex

        recipient = map_get(recip);
        if (!recipient)
        {
            return MAILBOX_INVALID;
        }
        else if (!recipient -> unblocked)
        {
            return MAILBOX_STOPPED;
        }
        else if (recipient -> msg_count < MAILBOX_SIZE)
        {
            newmsg = (message*)kmem_cache_alloc(messages, GFP_KERNEL);
            for(j=0; j<len; j++)
            {
                *((newmsg -> data) + j) = *((char*)msg + j);
            }
            recipient -> msg_count = recipient  -> msg_count + 1;
            newmsg    -> next      = recipient  -> contents;
            newmsg    -> sender = 0;
            newmsg    -> dest = recip;
            recipient -> contents  = newmsg;

            return 0;
        }

        //unlock the mutex
    }
    while(block);

	return MAILBOX_FULL;
}

asmlinkage long receive(pid_t* sender, void* msg, int* len, bool block)
{
    
    
    
    return 2;
}

asmlinkage long manage_mail(bool stop, int* vol)
{
    printk(KERN_INFO "manage");
    return 2;
}


 /*
 *=============================End New System Calle=============================
 *=============================Begin Syscall Table functions====================
 */


static unsigned long **find_sys_call_table(void)
{
	unsigned long int offset;

	for (offset=PAGE_OFFSET; offset<ULLONG_MAX; offset+=sizeof(void*))
	{
		if (((unsigned long **)offset)[__NR_close]==(unsigned long *)sys_close)
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
 *=============================End Syscall Table Functions======================
 *=============================Begin Module Functions===========================
 */


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

