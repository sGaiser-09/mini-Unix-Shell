/*
 * signals.c
 *
 * Implements signal handling
 *
 * @author Kaden Fieldhouse (kfieldh@pnw.edu)
 * @version 1
 *
 */

#include "../include/signals.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h> 

static void sigchld_handler(int sig)
 {
    int saved_errno = errno;
        while (waitpid(-1, NULL, WNOHANG) > 0) 
{
     
    }
    
    if (errno != ECHILD) 
{
       }
    errno = saved_errno; 
}

void setup_signal_handlers(void)
 {
    struct sigaction sa;
    
    
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; 

    if (sigaction(SIGCHLD, &sa, 0) == -1) 
{
        perror("myshell: sigaction failed for SIGCHLD");
          }
    
       if (signal(SIGINT, SIG_IGN) == SIG_ERR) 
{
        perror("myshell: signal failed for SIGINT");
    }
}