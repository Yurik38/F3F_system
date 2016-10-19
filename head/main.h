#ifndef _MAIN_H_
#define _MAIN_H_

extern __eeprom __no_init uchar eMode;
extern __eeprom __no_init uchar eLastSec;
extern __eeprom __no_init uchar eLaunchTime;
extern __eeprom __no_init uchar eTimeFormat;
extern __eeprom __no_init uchar eWeatherStation;
extern __eeprom __no_init uchar eComputerLink;

extern uchar volatile	Delay1;

void KeyHandler(void);
void PrintTime(uint timer, uchar format);

#endif //_MAIN_H_
