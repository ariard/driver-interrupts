#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/device.h>
#include "usb.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antoine Riard <ariard@student.42.fr>");
MODULE_DESCRIPTION("keylogger module");

# define KEYBOARD_VENDOR_ID	0x05ac
# define KEYBOARD_PRODUCT_ID	0x024f

# define KEYBOARD_MINOR_BASE	0

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

static struct file_operations keylogger_misc_fops = {
	.read = keylogger_read,
	.write = keylogger_write,
};

static struct miscdevice keylogger_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "module_keylogger",
	.fops = &keylogger_misc_fops,
};

static char *keyboard_devnode(struct device *dev, umode_t *mode)
{
	return kasprintf(GFP_KERNEL, "usb/%s", dev_name(dev));
}

static struct usb_class_driver keyboard_class = {
	.name = "keyboard-log",
	.devnode = keyboard_devnode,
	.fops = &keylogger_misc_fops,
	.minor_base = KEYBOARD_MINOR_BASE,
};

static int keyboard_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	static int	state;
	int		result;

	printk(KERN_INFO "keyboard driver : probing for %04X:%04X\n", id->idVendor, id->idProduct);
	if (!state) {
		result = usb_register_dev(interface, &keyboard_class);
		printk(KERN_INFO "keyboard driver : register %d\n", result);
		state = (result >= 0) ? 1 : 0;
	}
	// check if device already in driver list usb_driver->dynids
/*	usb_register_dev(struct usb_interface *intf, struct usb_class_driver *class_driver) */
	// kebyoard_driver		
	return 0;
}

static void keyboard_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "keyboard driver : keyboard remove\n");
	usb_deregister_dev(interface, &keyboard_class);
}

static struct usb_device_id keyboard_table[] = {
	{ USB_DEVICE(KEYBOARD_VENDOR_ID, KEYBOARD_PRODUCT_ID) },
/*	{ .driver_info = 42}, */
	{}
};

MODULE_DEVICE_TABLE(usb, keyboard_table);

static struct usb_driver keyboard_driver = {
	.name = "42keylogger",
	.id_table = keyboard_table,
	.probe = keyboard_probe,
	.disconnect = keyboard_disconnect,
};

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

/*	int driver_register(struct device_driver *drv) */
	result = usb_register(&keyboard_driver);
	if (result) {
		printk(KERN_INFO "driver keylogger : register failed with %d\n", result);
		goto err;
	}
	else {
		printk(KERN_INFO "driver keylogger : register successful\n");
	}
err:
	return result;
}

static void __exit keylogger_cleanup(void)
{
	misc_deregister(&keylogger_misc);
	printk(KERN_INFO "keylogger module : deregister\n");

/*	void driver_register(struct device_driver *drv) */
	usb_deregister(&keyboard_driver);
	printk(KERN_INFO "driver keylogger : register\n");
}

module_init(keylogger_init);
module_exit(keylogger_cleanup);
