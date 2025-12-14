/*
 * builtin.h
 *
 * Function prototypes and definitions for built-in shell commands.
 *
 * @author Kaden Fieldhouse (kfieldh@pnw.edu)
 * @version 1
 *
 */

#ifndef BUILTIN_H
#define BUILTIN_H

#include "parse.h" 


#define NUM_BUILTINS 1

char *builtin_str[] = {
    "cd"
};

//Function PROTOTYPES 

int is_builtin(Command *cmd);

int execute_builtin(Command *cmd);

int builtin_cd(Command *cmd);

#endif // BUILTIN_H