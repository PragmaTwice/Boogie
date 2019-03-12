#include "debug.h"
#include "char_dev.h"
#include "string.h"

// 全局字符设备链表
char_dev_t *char_devs;

// 字符设备初始化
void char_dev_init(void)
{
        char_dev_t *kb_dev = &kboard_dev;
        kb_dev->ops.init();
        add_char_dev(kb_dev);
}

// 内核注册字符设备
int add_char_dev(char_dev_t *cdev)
{
        char_dev_t *p = char_devs;
        while (p) {
                if (strcmp(p->name, cdev->name) == 0) {
                        return -1;
                }
                p = p->next;
        }

        cdev->next = char_devs;
        char_devs = cdev;

        return 0;
}
