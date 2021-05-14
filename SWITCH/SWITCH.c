#include <linux/module.h>
#include <linux/fs.h>
#include <asm/io.h>

#define NOD_MAJOR 100
#define NOD_NAME "SWITCH"

static volatile uint32_t *BASE;
static volatile uint32_t *GPFSEL1;
static volatile uint32_t *GPSET0;
static volatile uint32_t *GPCLR0;
static volatile uint32_t *GPIO_PUP_PDN_CNTRL_REG1;
static volatile uint32_t *GPLEV0;

MODULE_LICENSE("GPL");

static void ledOn(void);
static void ledOff(void);
static int getPushed(void);
// 0 is off, 1 is on
static int on = 0;

static int SWITCH_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "OPEN\n");
	return (0);
}

static int SWITCH_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "RELEASE\n");
	return (0);
}

static ssize_t SWITCH_read(struct file *filp, char *buf, size_t count, loff_t *ppos)
{
	printk(KERN_INFO "SHELL >> %s\n", buf);
	if (buf[0] == '1' && buf[1] == '\0')
		on = 1;
	else if (buf[0] == '2' && buf[1] == '\0')
		on = 0;
	return (count);
}

static ssize_t SWITCH_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
	printk(KERN_INFO "app message : %s\n", buf);
	return (count);
}

static long SWITCH_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd)
	{
		case 3 :
			ledOn();
			break;
		case 4 :
			ledOff();
			break;
		case 5 :
			getPushed();
			break;
	}
	return (0);
}

static struct file_operations fops = {
	.open = SWITCH_open,
	.release = SWITCH_release,
	.read = SWITCH_read,
	.write = SWITCH_write,
	.unlocked_ioctl = SWITCH_ioctl,
};

static void ledOn()
{
	printk(KERN_INFO "SWITCH ON\n");
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
	printk(KERN_INFO "SWITCH OFF\n");
	*GPCLR0 |= (1 << 18);
}

static int getPushed()
{
	//printk(KERN_INFO "::%X::\n", *GPLEV0);
	int ret = (int)(((*GPLEV0) >> 17) & 0x1);
	if (ret == 0)
	{
		printk(KERN_INFO "BUTTON CLICKED\n");
		if (on == 1)
		{
			on = 0;
			ledOff();
		}
		else
		{
			on = 1;
			ledOn();
		}
	}
	return (ret);
}

static int SWITCH_init(void)
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
	*GPFSEL1 &= ~(0x7 << 21);
	GPSET0 = BASE + (0x1C / 4);
	GPCLR0 = BASE + (0x28 / 4);
	GPLEV0 = BASE + (0x34 / 4);
	GPIO_PUP_PDN_CNTRL_REG1 = BASE + (0xE8 / 4);
	*GPIO_PUP_PDN_CNTRL_REG1 &= ~(0x3 << 2);
	*GPIO_PUP_PDN_CNTRL_REG1 |= (0x1 << 2);

	printk(KERN_INFO "::: %X ::: \n", ((*GPLEV0 >> 17) & 0x1));
	return (0);
}

static void SWITCH_exit(void)
{
	ledOff();
	iounmap(BASE);

	unregister_chrdev(NOD_MAJOR, NOD_NAME);
	printk(KERN_INFO "BYE\n");
}

module_init(SWITCH_init);
module_exit(SWITCH_exit);
