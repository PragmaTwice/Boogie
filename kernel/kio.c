#include "common.h"
#include "char_dev.h"
#include "kio.h"
#include "debug.h"

// 读取一个字符
char getchar(void)
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
