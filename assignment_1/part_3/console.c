//Ted Meyer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "proc_info_mgr.h"
#include "doit.h"
#include "proc_list.h" // for background tasks

char** split_args(char* buffer)
{
    char* clone = (char*)(malloc(128));
    int   count = 0;
    strncpy(clone, buffer, 128);

    char* toks = strtok (clone," ");
    while (toks != NULL)
    {
        count++;
        toks = strtok (NULL, " ");
    }
    free(toks);

    char** result = (char**)(malloc(sizeof(char*) * count));
    char** writer = result;

    toks = strtok (buffer," ");
    while (toks != NULL)
    {
        char* arg = (char*)(malloc(sizeof(char) * (strlen(toks) + 1)));
        strcpy(arg, toks);
        strcat(arg, "\0");
        *(writer++) = arg;
        toks = strtok (NULL, " ");
    }
    free(toks);

    return result;
}

int change_dir(char* dirname)
{
    char dir[1024];
    if (dirname)
    {
        chdir(dirname);
    }
    else
    {
        chdir("/");
    }

    if (getcwd(dir, sizeof(dir)) != NULL)
    {
        printf("%s\n", dir);
    }
}

int exeunt = 0;

void* listen_all(void* arg)
{
    proc_list** running = (proc_list**)(arg);
    while(!*running && exeunt == 0)
    {
        while(*running)
        {
            *running = prune(*running);
        }
    }
    
}

int main(int argc, char*argv[])
{
    printf("~~$ ");
    char* buffer = (char*)malloc(1024);
    int bufferspace = 0;
    proc_list* ptr = 0;
    proc_list** running = &ptr;
    *running = 0;
    int background = 0;

    pthread_t thread;
    pthread_create(&thread, NULL, listen_all, running);


    while(exeunt == 0)
    {
        char c = getchar();
        if (c == '\n')
        {
            if (bufferspace != 0)
            {
                char** args = split_args(buffer);

                if (strcmp("exit", args[0]) == 0)
                {
                    exeunt = 1;
                }
                else if (strcmp("cd", args[0]) == 0)
                {
                    change_dir(args[1]);
                }
                else
                {
                    
                    if (background)
                    {
                        *running = exec(args, running[0]);
                        printf("\n~~$ ");
                    }
                    else
                    {
                        proc_info* start = get_init();
                        print_info(exec_forg(args, start));
                    }
                }

                memset(buffer, 0, 128);
                bufferspace = 0;
            }
            else
            {
                printf("~~$ ");
            }
        }
        else if (c == EOF)
        {
            exeunt = 1;
            printf("\n");
        }
        else if (c != '\b')
        {
            background = (c=='&');
            if (!background)
            {
                buffer[(bufferspace++)%1024] = c;
            }
        }
    }

    printf("waiting for termination from all sub-processess");
    while(*running);

    return 0;
}
