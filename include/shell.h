#ifndef INCLUDE_SHELL_H_
#define INCLUDE_SHELL_H_

struct core_command
{
    char *name;
    int (*func)(char *);
};

struct command_list
{
    int size;
    struct core_command *list;
};

extern struct command_list commands;

void init_shell();

#endif 	// INCLUDE_SHELL_H_
