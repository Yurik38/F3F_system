#include "cpu.h"
#include "SndDrv.h"

uchar volatile	SndTime;
uchar volatile	Ring;
uchar volatile	Sound;

/************************************************************************/
void SndOn(uchar mask)
{
  _CLI();
  Sound = mask;
  _SndOn;
  SndTime = 20;
  _SEI();
}

/************************************************************************/
void SndOnRing(uchar len)
{
  _CLI();
  _SndOn;
  SndTime = 2;
  Ring = len;
  _SEI();
}
