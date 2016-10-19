#ifndef _SNDDRV_H_
#define _SNDDRV_H_

#define		SND_SHORT		0x00
#define		SND_LONG		0x0F
#define		SND_SHORT_SHORT	0x02
#define		SND_SHORT_LONG	0x1E

#define		_SndOn		{PORTC_Bit1 = 1; PORTD_Bit5 = 1; PORTD_Bit4 = 0}
#define		_SndOff		{PORTC_Bit1 = 0; PORTD_Bit5 = 0; PORTD_Bit4 = 1}


extern uchar volatile	SndTime;
extern uchar volatile	Ring;
extern uchar volatile	Sound;

#endif //_SNDDRV_H_
