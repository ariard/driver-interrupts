#include "keyboard.h"

static struct transition_table		scan_table [] = {
	{ 0x0, 0x10, 0x10, SUCCESS, 'q', PRESSED, "Q" },
	{ 0x0, 0x11, 0x11, SUCCESS, 'w', PRESSED, "W" },
	{ 0x0, 0x12, 0x12, SUCCESS, 'e', PRESSED, "E"},
	{ 0x0, 0x13, 0x13, SUCCESS, 'r', PRESSED, "R"},
	{ 0x0, 0x14, 0x14, SUCCESS, 't', PRESSED, "T"},
	{ 0x0, 0x15, 0x15, SUCCESS, 'y', PRESSED, "Y"},
	{ 0x0, 0x16, 0x16, SUCCESS, 'u', PRESSED, "U"},
	{ 0x0, 0x17, 0x17, SUCCESS, 'i', PRESSED, "I"},
	{ 0x0, 0x18, 0x18, SUCCESS, 'o', PRESSED, "O"},
	{ 0x0, 0x19, 0x19, SUCCESS, 'p', PRESSED, "P"},
	{ 0x0, 0x0, 0x0, ERROR },
};

void	scan_fsm_update(struct fsm *scan_fsm, unsigned int new)
{
	unsigned int	i = -1;	

	while (scan_table[++i].state != ERROR) {
		if (scan_table[i].ct == scan_fsm->ct && scan_table[i].input == new) {
			scan_fsm->ct = scan_table[i].output;
			scan_fsm->state = scan_table[i].state;
			scan_fsm->key = scan_table[i].key;
			scan_fsm->position = scan_table[i].position;
			scan_fsm->name = scan_table[i].name;
			break;
		}
	}
	if (scan_table[i].state == ERROR)
		scan_fsm->state = ERROR;
}

void	scan_fsm_clear(struct fsm *scan_fsm)
{
	scan_fsm->ct = 0;
	scan_fsm->state = UNDEFINED;
	scan_fsm->key = 0;
	scan_fsm->position = -1;
	scan_fsm->name = NULL;
}

void	scan_fsm_send(struct fsm *scan_fsm, void *target)
{
	struct timeval		tv;
	struct keystroke	*ks;

	if ((ks = kmalloc(sizeof(struct keystroke), GFP_KERNEL))) {
		ks->key = scan_fsm->key;
		/* array of flags shift, ctrl, caps + delay */
		ks->state = scan_fsm->position;
		strcpy(ks->name, scan_fsm->name);
		ks->ascii = (unsigned int)ks->key;	
		do_gettimeofday(&tv);
		ks->tv = tv;
		printk(KERN_INFO "key %c state %s name %s\n", ks->key, (ks->state == 0) ? "PRESSED" :
			"RELEASED", ks->name);
		INIT_LIST_HEAD(&ks->list);
		list_add_tail(&ks->list, target);
	}
}
