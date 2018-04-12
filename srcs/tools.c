#include "keyboard.h"

static struct scan_to_key	trl_table [] = {
	{ 0x10, 0x90, 'q' },
	{ 0x11, 0x91, 'w' },
	{ 0x12, 0x92, 'e' },
	{ 0x13, 0x93, 'r' },
	{ 0x14, 0x94, 't' },
	{ 0x15, 0x95, 'y' },
	{ 0x16, 0x96, 'u' },
	{ 0x17, 0x97, 'i' },
	{ 0x18, 0x98, 'o' },
	{ 0x19, 0x99, 'p' },
	{ 0x0, 0x0, 0 },
};

char	KEY(unsigned int value)
{
	unsigned int i = -1;	 	

	while (trl_table[++i].key)
		if (value == trl_table[i].st_code || value == trl_table[i].nd_code)
			return tr_table[i].key;
	
	return 0;
}

static struct scan_state	state_table[] = {
	{ 0x10, PRESSED },
	{ 0x90, RELEASED },
};

char	STATE(unsigned int value)
{
	unsigned int i = -1;	

	while (trl_table[++i].state != -1)
		if (value == state_table[i].scan_code)
			return state_table[i].state;
	return -1;
}
