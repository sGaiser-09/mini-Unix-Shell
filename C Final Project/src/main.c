/*
 * Main.java
 *
 * Main function, demo of algorithm
 *
 *
 * @author Kaden Fieldhouse (kfieldh@pnw.edu)
 * @version 2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parse.h"      
#include "../include/execute.h"    
#include "../include/signals.h"    
#include "../include/logger.h"

//Max Line Length for input buffer

#define MAX_LINE_LENGTH 1024

int main(void) 
{
 Command *cmd = NULL;

    char line[MAX_LINE_LENGTH];

setup_signal_handlers();

   if (open_log_file() != 0) 
{
        fprintf(stderr, "myshell: WARNING: Could not open log file\n");
    }

    printf("myshell started. Type 'exit' or 'quit' to close.\n");
    
        while (1) 
{
        //Display 
        printf("myshell> ");
        fflush(stdout); 

        //Read input 
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL)
 {
            // Handle EOF (Ctrl-D)
            printf("\nExiting myshell.\n");
            break;
        }

        // Remove trailing character and finds the index of '\n'
        line[strcspn(line, "\n")] = 0; 

        //Ignores empty lines
        if (strlen(line) == 0) 
{
            continue;
        }

        //Creating the exit and quit commands for user
        if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) 
{
//Exits the while loop
            break; 
        }

             cmd = parse_line(line);

        if (cmd != NULL) 
        {
            
            execute_command(cmd); 

           
            free_command(cmd);
        }
    }
close_log_file();
    return 0; 
}