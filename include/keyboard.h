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
# include <linux/spinlock.h>
# include <linux/time.h>
# include <linux/list.h>
# include <linux/slab.h>
# include <linux/uaccess.h>
# include <linux/rtc.h>
# include <linux/syscalls.h>
# include <linux/fcntl.h>
# include <linux/file.h>
# include <asm/io.h>

# define KEYBOARD_IRQ	1

# define SIZE		10000

/* Device Ports, Cmd, Data */

# define KEYBOARD_PORT	0x60
# define CTRL_PORT	0x64

# define ECHO		0xEE
# define SCAN_SET	0xF0

# define SCAN_SET_DATA	0x00
# define SCAN_SET_ONE	0x01
# define SCAN_SET_TWO	0x02
# define SCAN_SET_THREE	0x03

/* FSM State */

# define SUCCESS	0
# define ERROR		-1
# define UNDEFINED	1

/* Key State */

# define PRESSED	0
# define RELEASED	1

/* Flags State */

# define CAPSLOCK_SET	flags_array[0] & 1
# define SHIFTLOCK_SET	flags_array[1] & 1

# define SHIFT(x)	shift_table[(unsigned int)x]

/* Transition table special key value */

# define CAPSLOCK	1
# define LEFT_SHIFT	2
# define RIGHT_SHIFT	3
# define ESC		4
# define BACKSPACE	5
# define TAB		6
# define LEFT_CTRL	7
# define BACK		8
	/* resevered */
# define ENTER		13

struct transition_table {
	unsigned int		ct;
	unsigned int		input;
	unsigned int		output;
	int			state;
	char			key;
	char			position;
	char			*name;
};

struct fsm {
	unsigned int		ct;
	unsigned int 		state;
	char			key;	
	char			position;
	char			*name;
};

struct keystroke {
	unsigned char		key;
	unsigned char		state;
	char			name[25];
	unsigned int		ascii;
	struct rtc_time		tm;
	struct list_head	list;
};

void	scan_fsm_update(struct fsm *scan_fsm, unsigned int new);

void	scan_fsm_clear(struct fsm *scan_fsm);

void	scan_fsm_send(struct fsm *scan_fsm, void *target, rwlock_t *keylist_rwlock);

#endif
