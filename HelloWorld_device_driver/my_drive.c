#include <linux/module.h>
#include <linux/fs.h>

#define NOD_MAJOR 100
#define NOD_NAME "my_drive"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ujin");
MODULE_DESCRIPTION("my device driver");

static int my_drive_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "OPEN...OK\n");
	return (0);
}

static int my_drive_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "RELEASE...OK\n");
	return (0);
}

static struct file_operations fops = {
	.open = my_drive_open,
	.release = my_drive_release,
};

static int my_drive_init(void)
{
	if (register_chrdev(NOD_MAJOR, NOD_NAME, &fops) < 0)
	{
		printk("INIT FAIL\n");
		return (1);
	}
	printk(KERN_INFO "INIT OK\n");
	return (0);
}

static void my_drive_exit(void)
{
	unregister_chrdev(NOD_MAJOR, NOD_NAME);
	printk(KERN_INFO "EXIT\n");
}

module_init(my_drive_init);
module_exit(my_drive_exit);
