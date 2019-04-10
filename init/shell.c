#include "shell.h"
#include "debug.h"
#include "kio.h"
#include "ctype.h"
#include "string.h"

void parse_command(char *line, char **opcode, char **oprand)
{
    for(;*line != 0 && isspace(*line); ++line);
    *opcode = line;

    for(;*line != 0 && !isspace(*line); ++line);
    char *end = line;

    for(;*line != 0 && isspace(*line); ++line);
    *oprand = line;

    *end = 0;

}

int call_command(char *opcode, char *oprand)
{
    int i = 0;
    for(; i < commands.size; ++i)
    {
        if(strcmp(opcode, commands.list[i].name) == 0)
        {
            return commands.list[i].func(oprand);
        }
    }

    printk("error: cannot find \"%s\"\n", opcode);
    return -1;
}

void init_shell()
{
    console_clear();

    char line[1024];

    while(1)
    {
        printk_color(rc_black, rc_light_blue, "Boogie $ ");
        getstrln(line);
        
        char *opcode = 0, *oprand = 0;
        parse_command(line, &opcode, &oprand);

        call_command(opcode, oprand);
    }
}
