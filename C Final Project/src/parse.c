/*
 * parse.c
 *
 * 
 *
 *
 * @author Kaden Fieldhouse (kfieldh@pnw.edu)
 * @version 1
 *
 */
#include "../include/parse.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Initial capacity for the argv array 
#define INITIAL_ARGV_CAPACITY 8

//Cleans up the memorey
void free_command(Command *cmd) 
{
    if (cmd == NULL) 
{
        return;
    }

    //Frees the next command if pipe exists

    if (cmd->next != NULL) 
{
        free_command(cmd->next);
    }

   
    if (cmd->argv != NULL)
 {
        for (int i = 0; i < cmd->argc; i++) 
{
          
            if (cmd->argv[i] != NULL)
 {
                free(cmd->argv[i]);
            }
        }
       
        free(cmd->argv);
    }
    
        if (cmd->redir_file != NULL) 
{
        free(cmd->redir_file);
    }
if (cmd->raw_line != NULL) {
        free(cmd->raw_line);
    }
    
    free(cmd);
}


/**
 * @param line The raw input string read from the shell.
 * @return A pointer to the head Command structure, or NULL on error.
 */

Command *parse_line(const char *line)
 {
        char *s = strdup(line); 
    if (s == NULL) 
{
        perror("strdup failed");
        return NULL;
    }

    char *token;
    char *saveptr;
    
    //Allocate the main Command structure of the program

    Command *cmd = (Command *)calloc(1, sizeof(Command));
    if (cmd == NULL)
 { 
        perror("calloc failed"); 
        free(s); 
        return NULL; 
    }

cmd->raw_line = strdup(line);
    if (cmd->raw_line == NULL) {
        perror("strdup failed for raw_line");
        free(s);
        free(cmd);
        return NULL;
    }
    
    //Allocates the initial memory needed for argv and array
    int capacity = INITIAL_ARGV_CAPACITY;
    cmd->argv = malloc(capacity * sizeof(char *));
    if (cmd->argv == NULL)
 {
        perror("malloc failed");
        free(s);
        free(cmd);
        return NULL;
    }
    
    token = strtok_r(s, " ", &saveptr); 
    
    while (token != NULL) {
        
              
        // Redirection Operators (>, >>, <)
        
        if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0 || strcmp(token, "<") == 0) {
            
            //Sets the redirection type
            if (strcmp(token, ">") == 0)      cmd->redir_type = REDIRECT_OUT_TRUNC;
            else if (strcmp(token, ">>") == 0) cmd->redir_type = REDIRECT_OUT_APPEND;
            else if (strcmp(token, "<") == 0) cmd->redir_type = REDIRECT_IN;
            
            
            token = strtok_r(NULL, " ", &saveptr); 
            if (token == NULL) 
{
                fprintf(stderr, "myshell: Redirection file missing.\n");
                free(s); 
                free_command(cmd);                 
                return NULL; 
            }
            
            // Allocate and store the filename
          cmd->redir_file = strdup(token);
            if (cmd->redir_file == NULL)
 {
                perror("strdup failed for redir file");

                // Cleanup on allocation failure
                free(s); 
                free_command(cmd); 
                return NULL;
            }

        } else if (strcmp(token, "|") == 0)
 {
    // Pipe Operator (|)
    cmd->next = parse_line(saveptr);

        if (cmd->next == NULL) 
{
        free(s);
        free_command(cmd);
        return NULL; 
    }
            
    break; 
}
            
        
 else 
{
                       
            // Allocate memory for the token string and copy it as well
            cmd->argv[cmd->argc] = strdup(token); 
            if (cmd->argv[cmd->argc] == NULL)
 {
                perror("strdup failed for argument");
               // Cleans up upon allocation failure
                free(s);
                free_command(cmd);
                return NULL;
            }
            cmd->argc++;

            // Checks to see if capacity of argv needs to be expaneded 
            if (cmd->argc >= capacity) 
{
                capacity *= 2;
                char **new_argv = realloc(cmd->argv, capacity * sizeof(char *));
                if (new_argv == NULL)
 {
                    perror("realloc failed");
                    // Cleans up on realloc failure
                    free(s);
                    free_command(cmd); 
                    return NULL;
                }
                cmd->argv = new_argv;
            }
        }
        
        // Gets the next token from the original string
        token = strtok_r(NULL, " ", &saveptr);
    }
    
    //Finalizes argv array
    if (cmd->argv != NULL) 
{
              if (cmd->argc >= capacity) 
{
             cmd->argv = realloc(cmd->argv, (capacity + 1) * sizeof(char *));
        }
        cmd->argv[cmd->argc] = NULL;
    }
    
    //Cleans up the mutable copy of the input line
    free(s); 
    
    return cmd;
}