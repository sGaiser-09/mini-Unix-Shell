/*
 * builtin.c
 *
 * Implements the logic for built-in shell commands.
 *
 * @author Kaden Fieldhouse (kfieldh@pnw.edu)
 * @version 1
 *
 */

#include "../include/builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

int is_builtin(Command *cmd)
 {
    if (cmd == NULL || cmd->argv[0] == NULL) 
{
        return 0;
    }

       for (int i = 0; i < NUM_BUILTINS; i++) 
{
        if (strcmp(cmd->argv[0], builtin_str[i]) == 0)
 {
            return 1; 
        }
    }
    return 0; 
}

int execute_builtin(Command *cmd) 
{
    if (cmd == NULL || cmd->argv[0] == NULL) 
{
        return 0;
    }

    
    if (strcmp(cmd->argv[0], "cd") == 0) 
{
        return builtin_cd(cmd);
    }

    fprintf(stderr, "myshell: Internal error: unknown built-in command.\n");
    return 1;
}

int builtin_cd(Command *cmd) 
{
    char *path;

    if (cmd->argv[1] == NULL || strcmp(cmd->argv[1], "~") == 0)
 {
               path = getenv("HOME");
        if (path == NULL) 
{
            fprintf(stderr, "myshell: cd: HOME not set\n");
            return 1;
        }
    } else 
{
       
        path = cmd->argv[1];
    }

    
    if (chdir(path) != 0) 
{
        perror("myshell: cd failed");
        return 1;
    }

    return 0;
 }