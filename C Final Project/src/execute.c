/*
 * execute.c
 *
 * Implements the command execution logic (fork, execvp, pipes, redirection).
 *
 * @author Kaden Fieldhouse (kfieldh@pnw.edu)
 * @version 3
 *
 */

#include "../include/execute.h"
#include "../include/logger.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     
#include <sys/wait.h>  
#include <fcntl.h>      

int is_builtin(Command *cmd);
int execute_builtin(Command *cmd);


void execute_single_command(Command *cmd, int input_fd, int output_fd);
void handle_redirection(Command *cmd);
int execute_pipeline(Command *cmd, int input_fd);


/**
 * Executes a single command or a pipeline of commands.
 *
 * @return 0 on success, 1 on failure.
 */

int execute_command(Command *cmd)
 {
    if (cmd == NULL || cmd->argv[0] == NULL) 
{
        return 0;
    }

        if (is_builtin(cmd)) 
{
        return execute_builtin(cmd);
    }

    //PIPELINE HANDLING
    if (cmd->next != NULL) 
{
               return execute_pipeline(cmd, STDIN_FILENO);
    }
    
    //SINGLE COMMAND EXECUTION
       execute_single_command(cmd, STDIN_FILENO, STDOUT_FILENO);

    return 0;
}



 // Handles pipeline execution 
int execute_pipeline(Command *cmd, int input_fd) {
    int pipefd[2];
    int status;

    if (cmd->next == NULL)
 {
        
        execute_single_command(cmd, input_fd, STDOUT_FILENO);

        wait(&status); 
        return 0; 
    }

    
    if (pipe(pipefd) == -1) 
{
        perror("myshell: pipe failed");
        return 1;
    }

       execute_single_command(cmd, input_fd, pipefd[1]);

        close(pipefd[1]);

       int result = execute_pipeline(cmd->next, pipefd[0]);

       close(pipefd[0]);

        wait(&status);
    
    return result;
}


void execute_single_command(Command *cmd, int input_fd, int output_fd) 
{
    pid_t pid;
    int status;

        pid = fork();
    

    if (pid == 0)
 {
        
signal(SIGINT, SIG_DFL);

        //I/O Redirection
        if (input_fd != STDIN_FILENO) 
{
            dup2(input_fd, STDIN_FILENO); 
            if (input_fd > 2) close(input_fd); 
        }
        if (output_fd != STDOUT_FILENO) 
{
            dup2(output_fd, STDOUT_FILENO); 
            if (output_fd > 2) close(output_fd); 
        }

        //Handle Command Redirection (>, <, >>)
        handle_redirection(cmd);

        //Executes, this replaces the child process with the new program
        if (execvp(cmd->argv[0], cmd->argv) == -1) 
{
            perror("myshell: execution failed");
            exit(EXIT_FAILURE); 
        }
    } else if (pid < 0) 
{
        
        perror("myshell: fork failed");
    } else 
{
       
        
               if (!cmd->is_background) 
{
            
            if (waitpid(pid, &status, 0) == -1) 
{
                 perror("myshell: waitpid error");
            }

if (WIFEXITED(status) && cmd->raw_line != NULL) 
{ 
                log_command(pid, cmd->raw_line, WEXITSTATUS(status));
            }
                  } else {
             
             printf("myshell: Job started in background [%d]\n", pid);
         }
}
}
void handle_redirection(Command *cmd)
{
    if (cmd->redir_file == NULL) 
{
        return;
    }
    int flags = 0;
    int mode = 0; 
    int fd = -1;

    switch (cmd->redir_type) {

        case REDIRECT_IN:
            fd = open(cmd->redir_file, O_RDONLY);
            if (fd != -1) 
{
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            break;

        case REDIRECT_OUT_TRUNC:
         
            flags = O_WRONLY | O_CREAT | O_TRUNC;
            mode = 0644; 
            fd = open(cmd->redir_file, flags, mode);
            if (fd != -1) {
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            break;

        case REDIRECT_OUT_APPEND:
            
            flags = O_WRONLY | O_CREAT | O_APPEND;
            mode = 0644; 
            fd = open(cmd->redir_file, flags, mode);
            if (fd != -1) {
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            break;
            
        default:
           
            return;
    }

    if (fd == -1) {
        perror("myshell: Redirection failed");
       
        exit(EXIT_FAILURE); 
    }
}