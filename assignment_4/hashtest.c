#include "hashmap.h"
#include <stdio.h>

#include <sys/types.h>

int main()
{
    map_init();
    
    pid_t p0 = 0;
    pid_t p1 = 1;
    pid_t p2 = 2;
    pid_t p3 = 3;
    pid_t p4 = 4;
    pid_t p5 = 16;
    pid_t p6 = 17;
    pid_t p7 = 32;
    pid_t p8 = 33;
    pid_t p9 = 64;

    mailbox* m0 = (mailbox*)0;
    mailbox* m1 = (mailbox*)1;
    mailbox* m2 = (mailbox*)2;
    mailbox* m3 = (mailbox*)3;
    mailbox* m4 = (mailbox*)4;
    mailbox* m5 = (mailbox*)5;
    mailbox* m6 = (mailbox*)6;
    mailbox* m7 = (mailbox*)7;
    mailbox* m8 = (mailbox*)8;
    mailbox* m9 = (mailbox*)9;


    map_put(p0, m0);
    map_put(p1, m1);
    map_put(p2, m2);
    map_put(p3, m3);
    map_put(p4, m4);
    map_put(p5, m5);
    map_put(p6, m6);
    map_put(p7, m7);
    map_put(p8, m8);
    map_put(p9, m9);

    pid_t pa[10] = {p0,p1,p2,p3,p4,p5,p6,p7,p8,p9};
    int i;
    for(i=0; i<10; i++){
        printf("pid_%i: %i\n", i, (int)(map_get(pa[i])));
    }

    map_stop();
}
    
