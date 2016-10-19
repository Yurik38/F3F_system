#ifndef _LEDDRV_H_
#define _LEDDRV_H_

typedef enum
{
  GREEN_ON,
  RED_ON,
  ALL_OFF
}LEDCTRL;

void LedCtrl(LEDCTRL led);
#endif //_LEDDRV_H_
