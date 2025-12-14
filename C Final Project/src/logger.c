/*
 * logger.c
 *
 * Implements command logging using open() and write().
 *
 * @author Kaden Fieldhouse (kfieldh@pnw.edu)
 * @version 1
 *
 */

#include "../include/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     
#include <fcntl.h>     
#include <string.h>    
#include <sys/stat.h>   

int log_fd = -1;

#define MAX_LOG_LINE_SIZE 512
#define LOG_FILENAME "myshell.log"

int open_log_file(void)
{
   
    log_fd = open(LOG_FILENAME, O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (log_fd == -1)
    {
        perror("myshell: Failed to open log file");
        return -1;
    }
    return 0;
}

void close_log_file(void)
{
    if (log_fd != -1)
    {
        close(log_fd);
        log_fd = -1;
    }
}

void log_command(pid_t pid, const char *cmd_line, int status)
{
    char log_buffer[MAX_LOG_LINE_SIZE];
    int len;

   
    if (log_fd == -1)
    {
        return; 
    }

    
    len = snprintf(log_buffer, MAX_LOG_LINE_SIZE, "[pid=%d] cmd=\"%s\" status=%d\n",
                   (int)pid, cmd_line, status);

  
    if (len > 0 && len < MAX_LOG_LINE_SIZE) 
{
        if (write(log_fd, log_buffer, (size_t)len) == -1)
        {
                    }
    }
}