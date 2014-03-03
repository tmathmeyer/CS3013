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
#include <linux/sched.h>
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
        int    real_len;
        pid_t  sender;
} message;

// struct for mailbox (container)
typedef struct mailbox {
        pid_t owner;
        int msg_count;
        char unblocked;
        char deleted;
        atomic_t r_w;
        message* contents;
        wait_queue_head_t access;
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
int free_all_mail(mailbox* box);

// interceptor calls
static int  interceptor_start(void);
static void interceptor_end  (void);
mailbox* make_mailbox(pid_t pid);

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

//spinlock_t usps_lock;


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
            free_all_mail(del->box);
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
            free_all_mail(kill->box);
            kmem_cache_free(hashmap, kill);
        }
        i++;
    }
    kmem_cache_destroy(hashmap);
    return 0;
}

int free_all_mail(mailbox* box)
{
    while(box -> contents)
    {
        message* k = box -> contents;
        box -> contents = box -> contents -> next;
        kmem_cache_free(messages, k);
    }
    kmem_cache_free(mailboxes, box);
    return 0;
}


/*
 *=============================End Hashmap Functions============================
 *=============================Begin New System Calls===========================
 */


asmlinkage long send_message(pid_t recip, void* mesg, int len, bool block)
{
    mailbox* recipient;
    message* insert;
    message* msg;
    int count;
    
    recipient = map_get(recip);

    if (!recipient)
    {
        struct task_struct* ts = find_vpid(recip);
        if (ts -> pid != recip || ts -> mm == 0)
        {
            return MAILBOX_INVALID;
        }
        else
        {
            recipient = make_mailbox(recip);
            map_put(recip, recipient);
        }
    }

    do
    {
        printk("SENDING Q.length=%i\n", atomic_read(&(recipient -> r_w)));
        wait_event(recipient -> access, atomic_read(&(recipient -> r_w)) == 0);
        atomic_inc(&(recipient -> r_w));

        if (!(recipient -> deleted))
        {
            msg = (message*) kmem_cache_alloc(messages, GFP_KERNEL);
            msg -> next = 0;
            msg -> sender = current -> pid;
            msg -> real_len = len;
            for(count=0; count<len; count++)
            { //copy the memory
                *((msg -> data) + count) = *((char *)mesg + count);
            }

            insert = recipient -> contents;
            if (!recipient -> contents)
            {
                recipient -> contents = msg;
                recipient -> msg_count = recipient -> msg_count + 1;
                atomic_dec(&(recipient -> r_w));
                printk("the current value of the queue (before unlock): %i\n", atomic_read(&(recipient -> r_w)));
                wake_up(&(recipient -> access));
                return 0;
            } 
            else
            {
                insert = recipient -> contents;
                while(insert -> next)
                {
                    insert = insert -> next;
                }
                insert -> next = msg;
                recipient -> msg_count = recipient -> msg_count + 1;
                atomic_dec(&(recipient -> r_w));
                printk("the current value of the queue (before unlock): %i\n", atomic_read(&(recipient -> r_w)));
                wake_up(&(recipient -> access));
                return 0;
            }
        }
        else
        {
            printk("mailbox deleted???!?!?");
            atomic_dec(&(recipient -> r_w));
            wake_up(&(recipient -> access));
            return MAILBOX_INVALID;
        }
    }
    while(BLOCK);

    atomic_dec(&(recipient -> r_w));
    wake_up(&(recipient -> access));
    return MAILBOX_FULL;
}


int recv_enter(mailbox* m)
{
    return (atomic_read(&(m->r_w))==0 && m -> msg_count != 0) ||
            (m -> deleted);
}


asmlinkage long receive(pid_t* sender, void* mesg, int* len, bool block)
{
    mailbox* my_mail;
    message* msg;

    my_mail = map_get(current->pid);

    if (!my_mail)
    {
        my_mail = make_mailbox(current -> pid);
        map_put(current -> pid, my_mail);
    }

    if (my_mail -> msg_count == 0 && !block)
    {
        return MAILBOX_EMPTY;
    }
    
    do
    {
        printk("RECV blocking? %s\n", block?"yes":"no");
        printk("RECV Q.length=%i\n", atomic_read(&(my_mail -> r_w)));
        printk("     M.length=%i\n", my_mail -> msg_count);
        // entry condition:
        //     nobody in mail
        //     AND
        //     more than 0 entries
        //     OR
        //     mailbox deleted
        wait_event(my_mail -> access, recv_enter(my_mail) );
        atomic_inc(&(my_mail -> r_w));

        msg = my_mail -> contents;
        my_mail -> msg_count = my_mail -> msg_count + 1;
        
        if (msg)
        {
            if (copy_to_user(sender, &(msg->sender), sizeof(pid_t)))
            {
                printk(KERN_INFO "EFUALT @recieve_mail_1 EF_id: %i, proc: %i", EFAULT, current->pid);
                atomic_dec(&(my_mail -> r_w));
                my_mail -> contents = my_mail -> contents -> next;
                kmem_cache_free(messages, msg);
                wake_up(&(my_mail -> access));
                return MAILBOX_ERROR;
            }

            if (copy_to_user(mesg, msg->data, msg->real_len))
            {
                printk(KERN_INFO "EFUALT @recieve_mail_2 EF_id: %i, proc: %i", EFAULT, current->pid);
                atomic_dec(&(my_mail -> r_w));
                my_mail -> contents = my_mail -> contents -> next;
                kmem_cache_free(messages, msg);
                wake_up(&(my_mail -> access));
                return MAILBOX_ERROR;
            }

            if (copy_to_user(len, &(msg->real_len), sizeof(int)))
            {
                printk(KERN_INFO "EFUALT @recieve_mail_3 EF_id: %i, proc: %i", EFAULT, current->pid);
                atomic_dec(&(my_mail -> r_w));
                my_mail -> contents = my_mail -> contents -> next;
                kmem_cache_free(messages, msg);
                wake_up(&(my_mail -> access));
                return MAILBOX_ERROR;
            }

            my_mail -> contents = my_mail -> contents -> next;
            kmem_cache_free(messages, msg);
            atomic_dec(&(my_mail -> r_w));
            printk("about to free with Q length: %i\n", atomic_read(&(my_mail -> r_w)));
            wake_up(&(my_mail -> access));
            return 0;
        }
        else
        {
            printk("no messages!\n");
            my_mail->msg_count = 0;
        }
        atomic_dec(&(my_mail -> r_w));
        wake_up(&(my_mail -> access));
    }
    while(block);
    

    return MAILBOX_EMPTY;
}






asmlinkage long manage_mail(bool stop, int* vol)
{
    mailbox* my_mail = map_get(current->pid);
    
    if (my_mail)
    {
        wait_event(my_mail -> access, atomic_read(&(my_mail -> r_w)) == 0);
        atomic_inc(&(my_mail -> r_w));
        if (stop)
        {
            my_mail -> deleted = 1;
        }

        if (copy_to_user(vol, &(my_mail->msg_count), sizeof(int)))
        {
            printk(KERN_INFO "EFAULT @manage_mail EF_id: %i, proc: %i", EFAULT, current->pid);
            atomic_dec(&(my_mail -> r_w));
            wake_up(&(my_mail -> access));
            return MAILBOX_ERROR;
        }
    }
    else if (!stop)
    {
        my_mail = make_mailbox(current -> pid);
        atomic_inc(&(my_mail -> r_w));
        map_put(current -> pid, my_mail);

        if (copy_to_user(vol, &(my_mail->msg_count), sizeof(int)))
        {
            printk(KERN_INFO "EFAULT @manage_mail EF_id: %i, proc: %i", EFAULT, current->pid);
            atomic_dec(&(my_mail -> r_w));
            wake_up(&(my_mail -> access));
            return MAILBOX_ERROR;
        }
    }

    atomic_dec(&(my_mail -> r_w));
    wake_up(&(my_mail -> access));
    return 0;
}





mailbox* make_mailbox(pid_t pid)
{
    mailbox* my_mail = kmem_cache_alloc(mailboxes, GFP_KERNEL);
    my_mail -> owner      = pid;
    my_mail -> msg_count  = 0;
    my_mail -> unblocked  = 1;
    my_mail -> contents   = 0;
    my_mail -> deleted    = 0;
    atomic_set(&(my_mail -> r_w), 0);
    init_waitqueue_head(&(my_mail -> access));
    return my_mail;
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

    //initialize the //spinlock
    //spin_lock_init(&usps_lock);

    //spin_lock(&usps_lock);
    //swap the functions
    interceptor_start();

    //spin_unlock(&usps_lock);
    
    return 0;
}

static void __exit module_end(void)
{
    //spin_lock(&usps_lock);
    //swap functions back to saved state
    interceptor_end();

    //free all structs

    //close the caches (lock access)
    
    //some sort of lock thing here
    kmem_cache_destroy(mailboxes);
    kmem_cache_destroy(messages);
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

	printk(KERN_INFO "Postal Service has set up shop!\n");

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

