#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ujin");
MODULE_DESCRIPTION("my device driver");

static int mod_init(void)
{
	printk(KERN_INFO "\n");
	printk(KERN_INFO "Init OK\n");
	printk(KERN_INFO "...\n");
	printk(KERN_INFO "\n");
	return (0);
}

static void mod_exit(void)
{
	printk(KERN_INFO "\n");
	printk(KERN_INFO "Exit OK\n");
	printk(KERN_INFO "...\n");
	printk(KERN_INFO "\n");
}

module_init(mod_init);
module_exit(mod_exit);
