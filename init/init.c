#include "init.h"
#include "common.h"
#include "console.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "task.h"
#include "sched.h"
#include "char_dev.h"
#include "kio.h"

void show_init(const char *name, void (*init_func)())
{
    printk("  initializing %-30s ... ", name);
    init_func();
    printk_color(rc_black, rc_green, "(ok)\n", name);
}

void init_timer_200()
{
    init_timer(200);
}

void show_logo()
{
	printk_color(rc_black, rc_light_blue,
	"\t   ______                        ___      __  \n"
	"\t  (, /    )           ,        /(,  ) (__/  ) \n"
	"\t    /---(  _______       _    /    /    /     \n"
	"\t ) / ____)(_)(_)(_/__(__(/_  /    /  ) /      \n"
	"\t(_/ (          .-/          (___ /  (_/       \n"
	"\t              (_/                             \n"
	"\n"
	);
}

void kern_init()
{
	console_clear();

	show_logo();

	printk_color(rc_black, rc_light_brown, "Boogie Initialization:\n\n");

	show_init("Debug Module", init_debug);

	show_init("Global Descriptor Table", init_gdt);
	show_init("Interrupt Descriptor Table", init_idt);

	show_init("Timer Module", init_timer_200);

	show_init("Physical Memory Management", init_pmm);
	show_init("Virtual Memory Management", init_vmm);
	show_init("Heap Module", init_heap);

	show_init("Scheduler Module", init_sched);

	show_init("Character Device", char_dev_init);

	// 开启中断
	enable_intr();

	printk("%c\n", getchar());
	printk("hello\n");


	while (1) {
		cpu_hlt();
	}
}

