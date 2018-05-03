#include "keyboard.h"

static struct transition_table		scan_table [] = {
	{ 0x0, 0x1, 0x1, SUCCESS, ESC, PRESSED, "Escape" },
	{ 0x0, 0x2, 0x2, SUCCESS, '1', PRESSED, "1" },
	{ 0x0, 0x3, 0x3, SUCCESS, '2', PRESSED, "2" },
	{ 0x0, 0x4, 0x4, SUCCESS, '3', PRESSED, "3" },
	{ 0x0, 0x5, 0x5, SUCCESS, '4', PRESSED, "4" },
	{ 0x0, 0x6, 0x6, SUCCESS, '5', PRESSED, "5" },
	{ 0x0, 0x7, 0x7, SUCCESS, '6', PRESSED, "6" },
	{ 0x0, 0x8, 0x8, SUCCESS, '7', PRESSED, "7" },
	{ 0x0, 0x9, 0x9, SUCCESS, '8', PRESSED, "8" },
	{ 0x0, 0xA, 0xA, SUCCESS, '9', PRESSED, "9" },
	{ 0x0, 0xB, 0xB, SUCCESS, '0', PRESSED, "0" },
	{ 0x0, 0xC, 0xC, SUCCESS, '-', PRESSED, "-" },
	{ 0x0, 0xD, 0xD, SUCCESS, '=', PRESSED, "=" },
	{ 0x0, 0xE, 0xE, SUCCESS, BACK, PRESSED, "Backspace" },
	{ 0x0, 0xF, 0xF, SUCCESS, TAB, PRESSED, "Tab" },
	{ 0x0, 0x10, 0x10, SUCCESS, 'q', PRESSED, "Q" },
	{ 0x0, 0x11, 0x11, SUCCESS, 'w', PRESSED, "W" },
	{ 0x0, 0x12, 0x12, SUCCESS, 'e', PRESSED, "E" },
	{ 0x0, 0x13, 0x13, SUCCESS, 'r', PRESSED, "R" },
	{ 0x0, 0x14, 0x14, SUCCESS, 't', PRESSED, "T" },
	{ 0x0, 0x15, 0x15, SUCCESS, 'y', PRESSED, "Y" },
	{ 0x0, 0x16, 0x16, SUCCESS, 'u', PRESSED, "U" },
	{ 0x0, 0x17, 0x17, SUCCESS, 'i', PRESSED, "I" },
	{ 0x0, 0x18, 0x18, SUCCESS, 'o', PRESSED, "O" },
	{ 0x0, 0x19, 0x19, SUCCESS, 'p', PRESSED, "P" },
	{ 0x0, 0x1A, 0x1A, SUCCESS, '[', PRESSED, "[" },
	{ 0x0, 0x1B, 0x1B, SUCCESS, ']', PRESSED, "]" },
	{ 0x0, 0x1C, 0x1C, SUCCESS, ENTER, PRESSED, "Enter" },
	{ 0x0, 0x1D, 0x1D, SUCCESS, LEFT_CTRL, PRESSED, "left ctrl" },
	{ 0x0, 0x1E, 0x1E, SUCCESS, 'a', PRESSED, "A" },
	{ 0x0, 0x1F, 0x1F, SUCCESS, 's', PRESSED, "S" },
	{ 0x0, 0x20, 0x20, SUCCESS, 'd', PRESSED, "D" },
	{ 0x0, 0x21, 0x21, SUCCESS, 'f', PRESSED, "F" },
	{ 0x0, 0x22, 0x22, SUCCESS, 'g', PRESSED, "G" },
	{ 0x0, 0x23, 0x23, SUCCESS, 'h', PRESSED, "H" },
	{ 0x0, 0x24, 0x24, SUCCESS, 'j', PRESSED, "J" },
	{ 0x0, 0x25, 0x25, SUCCESS, 'k', PRESSED, "K" },
	{ 0x0, 0x26, 0x26, SUCCESS, 'l', PRESSED, "L" },
	{ 0x0, 0x27, 0x27, SUCCESS, ';', PRESSED, ";" },
	{ 0x0, 0x28, 0x28, SUCCESS, '\'', PRESSED, "\'" },
	{ 0x0, 0x29, 0x29, SUCCESS, '`', PRESSED, "`" },
	{ 0x0, 0x2A, 0x2A, SUCCESS, LEFT_SHIFT, PRESSED, "left shift" },
	{ 0x0, 0x2B, 0x2B, SUCCESS, '\\', PRESSED, "\\" },
	{ 0x0, 0x2C, 0x2C, SUCCESS, 'z', PRESSED, "Z" },
	{ 0x0, 0x2D, 0x2D, SUCCESS, 'x', PRESSED, "X" },
	{ 0x0, 0x2E, 0x2E, SUCCESS, 'c', PRESSED, "C" },
	{ 0x0, 0x2F, 0x2F, SUCCESS, 'v', PRESSED, "V" },
	{ 0x0, 0x30, 0x30, SUCCESS, 'b', PRESSED, "B" },
	{ 0x0, 0x31, 0x31, SUCCESS, 'n', PRESSED, "N" },
	{ 0x0, 0x32, 0x32, SUCCESS, 'm', PRESSED, "M" },
	{ 0x0, 0x33, 0x33, SUCCESS, ',', PRESSED, "," },
	{ 0x0, 0x34, 0x34, SUCCESS, '.', PRESSED, "." },
	{ 0x0, 0x35, 0x35, SUCCESS, '/', PRESSED, "/" },
	{ 0x0, 0x36, 0x36, SUCCESS, RIGHT_SHIFT, PRESSED, "right shift" },
	{ 0x0, 0x37, 0x37, SUCCESS, '*', PRESSED, "*" },
	{ 0x0, 0x38, 0x38, SUCCESS, LEFT_ALT, PRESSED, "left alt" },
	{ 0x0, 0x39, 0x39, SUCCESS, ' ', PRESSED, "space" },
	{ 0x0, 0x3A, 0x3A, SUCCESS, CAPSLOCK, PRESSED, "CapsLock" },
	{ 0x0, 0x3B, 0x3B, SUCCESS, F1, PRESSED, "F1" },
	{ 0x0, 0x3C, 0x3C, SUCCESS, F2, PRESSED, "F2" },
	{ 0x0, 0x3D, 0x3D, SUCCESS, F3, PRESSED, "F3" },
	{ 0x0, 0x3E, 0x3E, SUCCESS, F4, PRESSED, "F4" },
	{ 0x0, 0x3F, 0x3F, SUCCESS, F5, PRESSED, "F5" },
	{ 0x0, 0x40, 0x40, SUCCESS, F6, PRESSED, "F6" },
	{ 0x0, 0x41, 0x41, SUCCESS, F7, PRESSED, "F7" },
	{ 0x0, 0x42, 0x42, SUCCESS, F8, PRESSED, "F8" },
	{ 0x0, 0x43, 0x43, SUCCESS, F9, PRESSED, "F9" },
	{ 0x0, 0x44, 0x44, SUCCESS, F10, PRESSED, "F10" },
	{ 0x0, 0x45, 0x45, SUCCESS, NUMBERLOCK, PRESSED, "NumberLock" },
	{ 0x0, 0x46, 0x46, SUCCESS, SCROLLLOCK, PRESSED, "ScrollLock" },
	{ 0x0, 0x47, 0x47, SUCCESS, '7', PRESSED, "7" },
	{ 0x0, 0x48, 0x48, SUCCESS, '8', PRESSED, "8" },
	{ 0x0, 0x49, 0x49, SUCCESS, '9', PRESSED, "9" },
	{ 0x0, 0x4A, 0x4A, SUCCESS, '-', PRESSED, "-" },
	{ 0x0, 0x4B, 0x4B, SUCCESS, '4', PRESSED, "4" },
	{ 0x0, 0x4C, 0x4C, SUCCESS, '5', PRESSED, "5" },
	{ 0x0, 0x4D, 0x4D, SUCCESS, '6', PRESSED, "6" },
	{ 0x0, 0x4E, 0x4E, SUCCESS, '+', PRESSED, "+" },
	{ 0x0, 0x4F, 0x4F, SUCCESS, '1', PRESSED, "1" },
	{ 0x0, 0x50, 0x50, SUCCESS, '2', PRESSED, "2" },
	{ 0x0, 0x51, 0x51, SUCCESS, '3', PRESSED, "3" },
	{ 0x0, 0x52, 0x52, SUCCESS, '0', PRESSED, "0" },
	{ 0x0, 0x53, 0x53, SUCCESS, '.', PRESSED, "." },
	{ 0x0, 0x57, 0x57, SUCCESS, F11, PRESSED, "F11" },
	{ 0x0, 0x58, 0x58, SUCCESS, F12, PRESSED, "F12" },
	{ 0x0, 0xE0, 0xE0, VOID, K_ENTER, PRESSED, "Keyboard Enter"},
	{ 0x0, 0x1C, 0x1C, SUCCESS, K_ENTER, PRESSED, "Keyboard Enter"},

	/* Sep */
	{ 0x0, 0x81, 0x81, SUCCESS, ESC, RELEASED, "Escape" },
	{ 0x0, 0x82, 0x82, SUCCESS, '1', RELEASED, "1" },
	{ 0x0, 0x83, 0x83, SUCCESS, '2', RELEASED, "2" },
	{ 0x0, 0x84, 0x84, SUCCESS, '3', RELEASED, "3" },
	{ 0x0, 0x85, 0x85, SUCCESS, '4', RELEASED, "4" },
	{ 0x0, 0x86, 0x86, SUCCESS, '5', RELEASED, "5" },
	{ 0x0, 0x87, 0x87, SUCCESS, '6', RELEASED, "6" },
	{ 0x0, 0x88, 0x88, SUCCESS, '7', RELEASED, "7" },
	{ 0x0, 0x89, 0x89, SUCCESS, '8', RELEASED, "8" },
	{ 0x0, 0x8A, 0x8A, SUCCESS, '9', RELEASED, "9" },
	{ 0x0, 0x8B, 0x8B, SUCCESS, '0', RELEASED, "0" },
	{ 0x0, 0x8C, 0x8C, SUCCESS, '-', RELEASED, "-" },
	{ 0x0, 0x8D, 0x8D, SUCCESS, '=', RELEASED, "=" },
	{ 0x0, 0x8E, 0x8E, SUCCESS, BACK, RELEASED, "Backspace" },
	{ 0x0, 0x8F, 0x8F, SUCCESS, TAB, RELEASED, "Tab" },
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
	{ 0x0, 0x9A, 0x9A, SUCCESS, '[', RELEASED, "[" },
	{ 0x0, 0x9B, 0x9B, SUCCESS, ']', RELEASED, "]" },
	{ 0x0, 0x9C, 0x9C, SUCCESS, ENTER, RELEASED, "Enter" },
	{ 0x0, 0x9D, 0x9D, SUCCESS, LEFT_CTRL, RELEASED, "left ctrl" },
	{ 0x0, 0x9E, 0x9E, SUCCESS, 'a', RELEASED, "A" },
	{ 0x0, 0x9F, 0x9F, SUCCESS, 's', RELEASED, "S" },
	{ 0x0, 0xA0, 0xA0, SUCCESS, 'd', RELEASED, "D" },
	{ 0x0, 0xA1, 0xA1, SUCCESS, 'f', RELEASED, "F" },
	{ 0x0, 0xA2, 0xA2, SUCCESS, 'g', RELEASED, "G" },
	{ 0x0, 0xA3, 0xA3, SUCCESS, 'h', RELEASED, "H" },
	{ 0x0, 0xA4, 0xA4, SUCCESS, 'j', RELEASED, "J" },
	{ 0x0, 0xA5, 0xA5, SUCCESS, 'k', RELEASED, "K" },
	{ 0x0, 0xA6, 0xA6, SUCCESS, 'l', RELEASED, "L" },
	{ 0x0, 0xA7, 0xA7, SUCCESS, ';', RELEASED, ";" },
	{ 0x0, 0xA8, 0xA8, SUCCESS, '\'', RELEASED, "\'" },
	{ 0x0, 0xA9, 0xA9, SUCCESS, '`', RELEASED, "`" },
	{ 0x0, 0xAA, 0xAA, SUCCESS, LEFT_SHIFT, RELEASED, "left shift" },
	{ 0x0, 0xAB, 0xAB, SUCCESS, '\\', RELEASED, "\\" },
	{ 0x0, 0xAC, 0xAC, SUCCESS, 'z', RELEASED, "Z" },
	{ 0x0, 0xAD, 0xAD, SUCCESS, 'x', RELEASED, "X" },
	{ 0x0, 0xAE, 0xAE, SUCCESS, 'c', RELEASED, "C" },
	{ 0x0, 0xAF, 0xAF, SUCCESS, 'v', RELEASED, "V" },
	{ 0x0, 0xB0, 0xB0, SUCCESS, 'b', RELEASED, "B" },
	{ 0x0, 0xB1, 0xB1, SUCCESS, 'n', RELEASED, "N" },
	{ 0x0, 0xB2, 0xB2, SUCCESS, 'm', RELEASED, "M" },
	{ 0x0, 0xB3, 0xB3, SUCCESS, ',', RELEASED, "," },
	{ 0x0, 0xB4, 0xB4, SUCCESS, '.', RELEASED, "." },
	{ 0x0, 0xB5, 0xB5, SUCCESS, '/', RELEASED, "/" },
	{ 0x0, 0xB6, 0xB6, SUCCESS, RIGHT_SHIFT, RELEASED, "right shift" },
	{ 0x0, 0xB7, 0xB7, SUCCESS, '*', RELEASED, "*" },
	{ 0x0, 0xB8, 0xB8, SUCCESS, LEFT_ALT, RELEASED, "left alt" },
	{ 0x0, 0xB9, 0xB9, SUCCESS, ' ', RELEASED, "space" },
	{ 0x0, 0xBA, 0xBA, SUCCESS, CAPSLOCK, RELEASED, "CapsLock" },
	{ 0x0, 0xBB, 0xBB, SUCCESS, F1, RELEASED, "F1" },
	{ 0x0, 0xBC, 0xBC, SUCCESS, F2, RELEASED, "F2" },
	{ 0x0, 0xBD, 0xBD, SUCCESS, F3, RELEASED, "F3" },
	{ 0x0, 0xBE, 0xBE, SUCCESS, F4, RELEASED, "F4" },
	{ 0x0, 0xBF, 0xBF, SUCCESS, F5, RELEASED, "F5" },
	{ 0x0, 0xC0, 0xC0, SUCCESS, F6, RELEASED, "F6" },
	{ 0x0, 0xC1, 0xC1, SUCCESS, F7, RELEASED, "F7" },
	{ 0x0, 0xC2, 0xC2, SUCCESS, F8, RELEASED, "F8" },
	{ 0x0, 0xC3, 0xC3, SUCCESS, F9, RELEASED, "F9" },
	{ 0x0, 0xC4, 0xC4, SUCCESS, F10, RELEASED, "F10" },
	{ 0x0, 0xC5, 0xC5, SUCCESS, NUMBERLOCK, RELEASED, "NumberLock" },
	{ 0x0, 0xC6, 0xC6, SUCCESS, SCROLLLOCK, RELEASED, "ScrollLock" },
	{ 0x0, 0xC7, 0xC7, SUCCESS, '7', RELEASED, "7" },
	{ 0x0, 0xC8, 0xC8, SUCCESS, '8', RELEASED, "8" },
	{ 0x0, 0xC9, 0xC9, SUCCESS, '9', RELEASED, "9" },
	{ 0x0, 0xCA, 0xCA, SUCCESS, '-', RELEASED, "-" },
	{ 0x0, 0xCB, 0xCB, SUCCESS, '4', RELEASED, "4" },
	{ 0x0, 0xCC, 0xCC, SUCCESS, '5', RELEASED, "5" },
	{ 0x0, 0xCD, 0xCD, SUCCESS, '6', RELEASED, "6" },
	{ 0x0, 0xCE, 0xCE, SUCCESS, '+', RELEASED, "+" },
	{ 0x0, 0xCF, 0xCF, SUCCESS, '1', RELEASED, "1" },
	{ 0x0, 0xD0, 0xD0, SUCCESS, '2', RELEASED, "2" },
	{ 0x0, 0xD1, 0xD1, SUCCESS, '3', RELEASED, "3" },
	{ 0x0, 0xD2, 0xD2, SUCCESS, '0', RELEASED, "0" },
	{ 0x0, 0xD3, 0xD3, SUCCESS, '.', RELEASED, "." },
	{ 0x0, 0xD7, 0xD7, SUCCESS, F11, RELEASED, "F11" },
	{ 0x0, 0xD8, 0xD8, SUCCESS, F12, RELEASED, "F12" },
	{ 0x0, 0xE0, 0xE0, VOID, 0, UNDEFINED, ""},
	{ 0xE0, 0x1D, 0x1D, SUCCESS, RIGHT_CTRL, PRESSED, "right ctrl"},
	{ 0xE0, 0x38, 0x38, SUCCESS, RIGHT_ALT, PRESSED, "right alt"},
	{ 0xE0, 0x9D, 0x9D, SUCCESS, RIGHT_CTRL, RELEASED, "right ctrl"},
	{ 0xE0, 0xB8, 0xB8, SUCCESS, RIGHT_ALT, RELEASED, "right alt"},
	{ 0xE0, 0x5B, 0x5B, SUCCESS, LEFT_GUI, PRESSED, "left GUI"},
	{ 0xE0, 0x5C, 0x5C, SUCCESS, RIGHT_GUI, PRESSED, "right GUI"},
	{ 0xE0, 0xDB, 0xDB, SUCCESS, LEFT_GUI, RELEASED, "left GUI"},
	{ 0xE0, 0xDC, 0xDC, SUCCESS, RIGHT_GUI, RELEASED, "right GUI"},
	{ 0xE0, 0x48, 0x48, SUCCESS, CURSOR_UP, PRESSED, "cursor up"},
	{ 0xE0, 0x4B, 0x4B, SUCCESS, CURSOR_LEFT, PRESSED, "cursor left"},
	{ 0xE0, 0x4D, 0x4D, SUCCESS, CURSOR_RIGHT, PRESSED, "cursor right"},
	{ 0xE0, 0x50, 0x50, SUCCESS, CURSOR_DOWN, PRESSED, "cursor down"},
	{ 0xE0, 0xC8, 0xC8, SUCCESS, CURSOR_UP, RELEASED, "cursor up"},
	{ 0xE0, 0xCB, 0xCB, SUCCESS, CURSOR_LEFT, RELEASED, "cursor left"},
	{ 0xE0, 0xCD, 0xCD, SUCCESS, CURSOR_RIGHT, RELEASED, "cursor right"},
	{ 0xE0, 0xD0, 0xD0, SUCCESS, CURSOR_DOWN, RELEASED, "cursor down"},
	{ 0xE0, 0x47, 0x47, SUCCESS, HOME, PRESSED, "home"},
	{ 0xE0, 0x49, 0x49, SUCCESS, PAGE_UP, PRESSED, "page up"},
	{ 0xE0, 0x4F, 0x4F, SUCCESS, END, PRESSED, "end"},
	{ 0xE0, 0x51, 0x51, SUCCESS, PAGE_DOWN, PRESSED, "page down"},
	{ 0xE0, 0x53, 0x53, SUCCESS, DELETE, PRESSED, "delete"},
	{ 0xE0, 0xC7, 0xC7, SUCCESS, HOME, RELEASED, "home"},
	{ 0xE0, 0xC9, 0xC9, SUCCESS, PAGE_UP, RELEASED, "page up"},
	{ 0xE0, 0xCF, 0xCF, SUCCESS, END, RELEASED, "end"},
	{ 0xE0, 0xD1, 0xD1, SUCCESS, PAGE_DOWN, RELEASED, "page down"},
	{ 0xE0, 0xD3, 0xD3, SUCCESS, DELETE, RELEASED, "delete"},
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

static char 		flags_array [] = { 0x0, 0x0, 0x0};

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
		flags_array[1] = 1;
	}
	else if ((scan_fsm->key == LEFT_SHIFT || scan_fsm->key == RIGHT_SHIFT)
		&& scan_fsm->position == RELEASED) {
		flags_array[1] = 0;
	}

	if ((scan_fsm->key == LEFT_CTRL || scan_fsm->key == RIGHT_CTRL)
		&& scan_fsm->position == PRESSED) {
		flags_array[2] = 1;
	}
	else if ((scan_fsm->key == LEFT_CTRL || scan_fsm->key == RIGHT_CTRL) 
		&& scan_fsm->position == RELEASED) {
		flags_array[2] = 0;
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

	if (CAPSLOCK_SET && (value > 32 && value < 127))
		value -= 32;
	if (SHIFTLOCK_SET && (value > 32 && value < 127))
		value = SHIFT(key);
	if (CTRLOCK_SET && (value > 32 && value < 127))
		value = 0;
	
	return value;
}
 
void			scan_fsm_send(struct fsm *scan_fsm, void *target, rwlock_t *keylist_rwlock)
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
		write_lock(keylist_rwlock);
		list_add_tail(&ks->list, target);
		write_unlock(keylist_rwlock);
	}
}

/*		printk(KERN_INFO "[%04d-%02d-%02d %02d:%02d:%02d] key %c state %s name %s\n", tm.tm_year + 1900, tm.tm_mon + 1,
			tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ks->key, (ks->state == 0) ? "PRESSED" :
			"RELEASED", ks->name); */
