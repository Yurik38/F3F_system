#include "cpu.h"
#include "LedDrv.h"

/************************************************************************/
void LedCtrl(LEDCTRL led)
{
  if (led == GREEN_ON)
  {
    PORTC_Bit3 = 1;
    PORTC_Bit2 = 0;
  }
  else if (led == RED_ON)
  {
    PORTC_Bit3 = 0;
    PORTC_Bit2 = 1;
  }
  else
  {
    PORTC_Bit3 = 0;
    PORTC_Bit2 = 0;
  }
}
