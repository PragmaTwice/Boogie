#include "common.h"
#include "char_dev.h"
#include "kio.h"
#include "debug.h"
#include "ctype.h"
#include "string.h"

// 读取一个字符
char getch(void)
{
        char ch;
        char_dev_t *kb_dev = &kboard_dev;

        if (!kb_dev->ops.device_valid()) {
                return 0;
        }

        while (kb_dev->ops.read(&ch, 1) == 0) {
                cpu_hlt();
        }
        
        return ch;
}

// 读取一个字符，并回显
char getchar(void)
{
        char ch = getch();

        if(ch != 0) {
                console_putc_color(ch, rc_black, rc_white);
        }
        if(ch == 8) {
                console_write(" \10");
        }
        return ch;
}

// 读取字符串直到回车
void getstrln(char* str)
{
        char ch;
        while((ch = getchar()) != 0 && ch != '\n') {
                if(ch == 8) {
                        str--;
                }
                else {
                        *str = ch;
                        str ++;
                }
                
        }

        *str = 0;
}
