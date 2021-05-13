#include <linux/module.h>
#include <linux/fs.h>
#include <asm/io.h>

#define NOD_MAJOR 100
#define NOD_NAME "LED"

static volatile uint32_t *BASE;
static volatile uint32_t *GPFSEL1;
static volatile uint32_t *GPSET0;
static volatile uint32_t *GPCLR0;

MODULE_LICENSE("GPL");

static void ledOn(void);
static void ledOff(void);

static int LED_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "OPEN\n");
	return (0);
}

static int LED_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "RELEASE\n");
	return (0);
}

static ssize_t LED_read(struct file *filp, char *buf, size_t count, loff_t *ppos)
{
	buf[0] = 'H';
	buf[1] = 'I';
	buf[2] = '\0';
	return (count);
}

static ssize_t LED_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
	printk(KERN_INFO "APP MESSAGE : %s\n", buf);
	return (count);
}

static long LED_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_ALERT "%d\n", cmd);
	switch (cmd)
	{
		case 3 :
			printk(KERN_INFO "IOCTL 3\n");
			break;
		case 4 :
			printk(KERN_INFO "IOCTL 4\n");
			break;
		case 5 :
			printk(KERN_INFO "IOCTL 5\n");
			break;
		case 6 :
			ledOn();
			break;
		case 7 :
			ledOff();
			break;
	}
	return (0);
}

static struct file_operations fops = {
	.open = LED_open,
	.release = LED_release,
	.read = LED_read,
	.write = LED_write,
	.unlocked_ioctl = LED_ioctl,
};

static void ledOn()
{
	printk(KERN_INFO "LED ON\n");
	// output setting
	// 24 ~ 26bit is 18 pin set to 0
	*GPFSEL1 &= ~(0x7 << 24);
	// 24bit set to 1
	*GPFSEL1 |= (1 << 24);
	// set
	*GPSET0 |= (1 << 18);
}

static void ledOff()
{
	printk(KERN_INFO "LED OFF\n");
	*GPCLR0 |= (1 << 18);
}

static int LED_init(void)
{
	if (register_chrdev(NOD_MAJOR, NOD_NAME, &fops) < 0)
	{
		printk("INIT FAIL\n");
		return (1);
	}

	printk(KERN_INFO "HI\n");
	BASE = (uint32_t *)ioremap(0xFE200000, 256);
	// 4 means 4Byte(int) 
	// base is int
	GPFSEL1 = BASE + (0x04 / 4);
	GPSET0 = BASE + (0x1C / 4);
	GPCLR0 = BASE + (0x28 / 4);
	return (0);
}

static void LED_exit(void)
{
	ledOff();
	iounmap(BASE);

	unregister_chrdev(NOD_MAJOR, NOD_NAME);
	printk(KERN_INFO "BYE\n");
}

module_init(LED_init);
module_exit(LED_exit);
