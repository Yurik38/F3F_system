#ifndef _EVENT_H_
#define _EVENT_H_

/* Command from main device */
typedef enum Commands
{
	CMD_NONE = 0,
	
	CMD_START_ROUND,
	CMD_CANCEL,
	CMD_NUM_PACK_Q,
	CMD_BAT_VOLT_Q,
	CMD_SOUND,
 	CMD_READY_TIME_OUT,
	CMD_INFO_Q,
/*Internal command and*/
	CMD_NEXT,
	CMD_PREV,
	CMD_STOP,
/* Command from points */
	CMD_TIME_STAMP,
	CMD_NUM_PACKET,
	CMD_VOLTAGE,
	CMD_LETTER,
	CMD_END_CHAR,
	MAX_NUMBER_CMD,
}T_COMMAND;

typedef struct
{
	T_COMMAND	cmd;
	void*		param0;
	uchar		addr;
}T_EVENT;

void InitEventList(void);
void PostEvent(T_COMMAND cmd, void* param, uchar addr);
T_EVENT* GetEvent(void);
//T_EVENT* GetCurEventAddr(void);

#endif //_EVENT_H_
