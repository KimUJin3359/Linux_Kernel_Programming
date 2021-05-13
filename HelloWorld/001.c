#include <linux/module.h>

MODULE_LICENSE("GPL");

static int hi_init(void)
{
	printk(KERN_INFO "OK HELLO\n");
	return (0);
}

static void hi_exit(void)
{
	printk(KERN_INFO "BYE\n");
}

module_init(hi_init);
module_exit(hi_exit);
