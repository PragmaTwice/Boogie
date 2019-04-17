#include "shell.h"
#include "debug.h"
#include "kio.h"
#include "ctype.h"
#include "string.h"

// returns length of words
int parse_command(char *line, char *words[])
{
    // status machine
    enum { word, space, quoted_begin, quoted } status = space;

    int length = 0;
    for(char *i = line; *i != 0; ++i)
    {
        switch(status)
        {
            case word:
                if(isspace(*i))
                {
                    status = space;
                    *i = 0;
                }

                break;
            case space:
                if(!isspace(*i))
                {
                    if(*i == '"')
                    {
                        status = quoted_begin;
                    }
                    else
                    {
                        status = word;
                        *words = i;
                        words++;
                        length++;
                    }
                    
                }
                
                break;
            case quoted_begin:
                if(*i == '"')
                {
                    status = space;
                    *i = 0;
                    *words = i;
                    words++;
                    length++;
                }
                else
                {
                    status = quoted;
                    *words = i;
                    words++;
                    length++;
                }
                

                break;
            case quoted:
                if(*i == '"')
                {
                    status = space;
                    *i = 0;
                }

                break;
        }
    }

    return length;

}

int call_command(int length, char *words[])
{
    int i = 0;
    for(; i < commands.size; ++i)
    {
        if(strcmp(words[0], commands.list[i].name) == 0)
        {
            words ++;
            length --;
            return commands.list[i].func(length, words);
        }
    }

    printk("error: cannot find \"%s\"\n", words[0]);
    return -1;
}

void init_shell()
{
    console_clear();

    char line[1024];

    char *words [1024];
    int length = 0;

    while(1)
    {
        printk_color(rc_black, rc_light_blue, "Boogie $ ");
        getstrln(line);
        
        length = parse_command(line, words);

        call_command(length, words);
    }
}
