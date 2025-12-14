/*
 * parse.h
 *
 *  
 *
 * @author Kaden Fieldhouse (kfieldh@pnw.edu)
 * @version 1
 *
 */

#ifndef PARSE_H
#define PARSE_H

#include <stddef.h> 

//Enum for Redirection Types, this helps the user to know what I/0 redirect to use. 
typedef enum {
    REDIRECT_NONE,         
    REDIRECT_OUT_TRUNC,   
    REDIRECT_OUT_APPEND,   
    REDIRECT_IN            
} 
RedirectionType;

//Main Command Structure 
typedef struct Command 
{
    char **argv;              
    int argc;                 
    
    //I/O Redirection
    RedirectionType redir_type;
    char *redir_file;        

    //Pipes
        struct Command *next;     
    
   
    int is_background; 

char *raw_line;       
} 
Command;



Command *parse_line(const char *line);

// Cleans up all the memory used by the command structures
void free_command(Command *cmd);

#endif 