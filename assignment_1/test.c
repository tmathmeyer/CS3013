#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/select.h>

void * entry_point(void * p) {
        int readpipe = *(int *)p;
            fd_set rfds;

                char c;

                    for (;;) {
                                FD_ZERO(&rfds);
                                        FD_SET(STDIN_FILENO, &rfds);
                                                FD_SET(readpipe, &rfds);

                                                        while (select(readpipe + 1, &rfds, NULL, NULL, NULL) == 0);

                                                                if (FD_ISSET(readpipe, &rfds)) {
                                                                                close(readpipe);
                                                                                            break;
                                                                                                    }

                                                                if (FD_ISSET(STDIN_FILENO, &rfds)) {
                                                                                if (read(STDIN_FILENO, &c, sizeof(c)) > 0) {
                                                                                                    printf("Read: %d\n", c);
                                                                                                                }
                                                                                        }
                                                                    }

                        printf("Thread terminating\n");

                            pthread_exit(NULL);
}

int main() {
        pthread_t thread;
            int r;
                int pipes[2];

                    pipe(pipes);

                        if (r = pthread_create(&thread, NULL, entry_point, &pipes[0])) {
                                    printf("Error: %d\n", r);
                                            return 1;
                                                }

                            sleep(5);

                                printf("Closing pipe and joining thread.\n");

                                    close(pipes[1]);
                                        pthread_join(thread, NULL);

                                            pthread_exit(NULL);
}
