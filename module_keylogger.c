#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/device.h>

MODULE_LICENCE("GPL");
MODULE_AUTHOR("Antoine Riard <ariard@student.42.fr>");
MODULE_DESCRIPTION("keylogger module");

static ssize_t keylogger_read(struct file *filp, char __user *buffer,
				size_t length, loff_t *offset)
{
	ssize_t		retval = 0;

	return retval;
}

static ssize_t keylogger_write(struct file *filp, const char __user *buffer,
				size_t length, loff_t *offset)
{
	ssize_t		retval = 0;

	return retval;
}

static struct file_operations misc_keylogger_fops = {
	.read = keylogger_read,
	.write = keylogger_write,
};

static struct miscdevice misc_keylogger = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "module_keylogger",
	.fops = &keylogger_fops,
};

static struct device_driver keylogger_driver = {
	.name = "42keylogger",
	.bus = &pci_bus_type,
};

static int __init keylogger_init(void)
{
	int	result;

	result = misc_register(misc_keylogger);
	if (result)
		printk(KERN_INFO "keylogger module : register failed with %d\n", result);
	else
		printk(KERN_INFO "keylogger module : register successful\n");

/*	int driver_register(struct device_driver *drv) */

	return result;
}

static void __exit keylogger_cleanup(void)
{
	misc_deregister(&misc_keylogger);
	printk(KERN_INFO "keylogger module : deregister\n");

/*	void driver_register(struct device_driver *drv) */

}


module_init(keylogger_init);
module_exit(keylogger_cleanup);
