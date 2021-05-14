#include <linux/module.h>
#include <linux/fs.h>
#include <asm/io.h>

#define NOD_MAJOR 100
#define NOD_NAME "ioctl_test"

MODULE_LICENSE("GPL");

static int ioctl_test_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "OPEN\n");
	return (0);
}

static int ioctl_test_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "RELEASE\n");
	return (0);
}

static long ioctl_test_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_ALERT "%d\n", cmd);
	switch (cmd)
	{
		case _IO(777, 3) :
			printk(KERN_INFO "IOCTL 3\n");
			break;
		case _IO(777, 4) :
			printk(KERN_INFO "IOCTL 4\n");
			break;
		case _IO(777, 5) :
			printk(KERN_INFO "IOCTL 5\n");
			break;
	}
	return (0);
}

static struct file_operations fops = {
	.open = ioctl_test_open,
	.release = ioctl_test_release,
	.unlocked_ioctl = ioctl_test_ioctl,
};

static int ioctl_test_init(void)
{
	if (register_chrdev(NOD_MAJOR, NOD_NAME, &fops) < 0)
	{
		printk("INIT FAIL\n");
		return (1);
	}

	printk(KERN_INFO "HI\n");
	return (0);
}

static void ioctl_test_exit(void)
{
	unregister_chrdev(NOD_MAJOR, NOD_NAME);
	printk(KERN_INFO "BYE\n");
}

module_init(ioctl_test_init);
module_exit(ioctl_test_exit);
