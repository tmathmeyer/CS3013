/* Ted Meyer
 * hashmap.c
 * 2014-02-23
 * License: GPLv2
 */

#include <stdlib.h>
#include "mailstructs.h"
#include "hashmap.h"

//globals
map_elem** map;

//implementations
int map_init()
{
    map = (map_elem**) malloc(MAP_SIZE * sizeof(map_elem*));
    int i; for(i=0; i<MAP_SIZE; i++)
    {
        *(map + i) = 0;
    }
}

int map_put(pid_t id, mailbox* box)
{
    map_elem* list = *(map + id%MAP_SIZE);
    map_elem* n =  malloc(sizeof(map_elem));
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
    }
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
            free(del);
            return 0;
        }
    }
    return 0;
}

map_stop()
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
            free(kill);
        }
        i++;
    }
    return 0;
}

