/*#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/irq.h> */
#include "keyboard.h"

static char *id = "keyboard";

static struct miscdevice keylogger_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "module_keylogger",
	.fops = &keylogger_misc_fops,
};

irqreturn_t	keyboard_interrupt(int irq, void *dev_id)
{
	printk(KERN_INFO "into keyboard interrupt");
	return IRQ_NONE;
	printk(KERN_INFO "dev id is %s\n", (char *)dev_id);
	return IRQ_HANDLED;
}

static int __init keylogger_init(void)
{
	int	result;

	result = misc_register(&keylogger_misc);
	if (result) {
		printk(KERN_INFO "misc keylogger : register failed with %d\n", result);
		goto err;
	}
	else {
		printk(KERN_INFO "misc keylogger : register successful\n");
	}

	result = request_irq(KEYBOARD_IRQ, keyboard_interrupt, IRQF_SHARED, "keyboard", id);
	if (result) {
		printk(KERN_INFO "driver keylogger : IRQ register failed");
		
		goto err;
	}

	return result;


err:
	misc_deregister(&keylogger_misc);
	return result;
}

static void __exit keylogger_cleanup(void)
{
	misc_deregister(&keylogger_misc);
	printk(KERN_INFO "keylogger module : deregister\n");

/*	void driver_register(struct device_driver *drv) */

	free_irq(KEYBOARD_IRQ, id);
}

module_init(keylogger_init);
module_exit(keylogger_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antoine Riard <ariard@student.42.fr>");
MODULE_DESCRIPTION("keylogger module");
