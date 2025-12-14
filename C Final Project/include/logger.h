/*
 * logger.h
 *
 * Function prototypes and definitions for command logging
 *
 * @author Kaden Fieldhouse (kfieldh@pnw.edu)
 * @version 1
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <sys/types.h> 

extern int log_fd; 

int open_log_file(void);

void close_log_file(void);

void log_command(pid_t pid, const char *cmd_line, int status);

#endif // LOGGER_H