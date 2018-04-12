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

struct transition_table {
	unsigned int	ct;
	unsigned int	input;
	unsigned int	output;
	int		state;
	char		key;
	char		position;
	char		*name;
};

struct fsm {
	unsigned int	ct;
	unsigned int 	state;
	char		key;	
	char		position;
	char		*name;
};

struct keystroke {
	unsigned char		key;
	unsigned char		state;
	char			name[25];
	struct timeval		tv;
	struct list_head	list;
};

void	scan_fsm_update(struct fsm *scan_fsm, unsigned int new);

void	scan_fsm_clear(struct fsm *scan_fsm);

void	scan_fsm_send(struct fsm *scan_fsm, void *target);


#endif
