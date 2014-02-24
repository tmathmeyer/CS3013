/* Ted Meyer
 * hashmap.h
 * 2014-02-24
 * License GPLv2
 */

#ifndef __HASHMAP__
#define __HASHMAP__

#include "mailstructs.h"

#define MAP_SIZE 16

typedef struct map_elem {
        struct map_elem* next;
        pid_t            id;
        mailbox*         box;
} map_elem;


int map_init(void);
int map_put(pid_t, mailbox* box);
int map_rem(pid_t id);
int map_stop(void);
mailbox* map_get(pid_t id);


#endif
