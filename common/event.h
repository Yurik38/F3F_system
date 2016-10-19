#ifndef _EVENT_H_
#define _EVENT_H_

/* Command from main device */
typedef enum Commands
{
	CMD_NONE = 0,
	
	CMD_START_ROUND,	//0x01
	CMD_CANCEL,
	CMD_TIME_STAMP,
	CMD_WEATHER_STAMP,
	CMD_INFO_Q,
	CMD_WEATHER_Q,		//0x05
	CMD_NEXT,
	CMD_PREV,
	CMD_STOP,
	CMD_COMP_ACK = 0x11,
	CMD_WEATHER,
	CMD_INFO,
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
