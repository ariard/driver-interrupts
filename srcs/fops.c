#include "keyboard.h"

ssize_t keylogger_read(struct file *filp, char __user *buffer,
				size_t length, loff_t *offset)
{
	ssize_t		retval = 0;

	return retval;
}

ssize_t keylogger_write(struct file *filp, const char __user *buffer,
				size_t length, loff_t *offset)
{
	ssize_t		retval = 0;

	return retval;
}
