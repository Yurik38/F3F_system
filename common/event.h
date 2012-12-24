#ifndef _EVENT_H_
#define _EVENT_H_

/* Command from main device */
#define		START_ROUND	0x01
#define		CANCEL		0x02
#define		NUM_PACK_Q	0x03
#define		BAT_VOLT_Q	0x04
#define		SOUND		0x05
#define 	READY_TIME_OUT	0x06
#define		INFO_Q		0x07
/*Internal command and*/
#define		NEXT		0x08
#define		PREV		0x09
#define		STOP		0x0A
/* Command from points */
#define		TIME_STAMP	0x10
#define		NUM_PACKET	0x11
#define		VOLTAGE		0x12
#define		LETTER		0x13
#define		END_CHAR	0x14

typedef struct
{
	uchar		cmd;
	uint		param0;
	uchar		addr;
}T_EVENT;

void InitEventList(void);
void PostEvent(uchar cmd, uint param, uchar addr);
T_EVENT* GetEvent(void);
T_EVENT* GetCurEventAddr(void);

#endif //_EVENT_H_
