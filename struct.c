#include <linux/kernel.h>
#include <linux/module.h.>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/timekeeping.h>
#include <linux/slob.h>
#include <linux/time.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

static struct		s_stroke {
	unsigned char	key;
	unsigned char	state;
	char		name[25];
	char		value;
	struct tm 	tm;
	struct s_stroke	*next;
}
