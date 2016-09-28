/*
Quatrz - 11059200 Hz
Fuses
OSCCAL  = AF, AE, A9, AB
BLEV    = 0
BODEN   = 1
SUT     = 3
CKSEL   = F
BLB1    = 3
BLB0    = 3
OCDEN   = 1
JTAGEN  = 1
CKOPT   = 1
EESV    = 0
BSIZ    = 1
BRST    = 1
*/

#include "cpu.h"
#include "ind2_16.h"
#include <stdio.h>
#include <string.h>
//#include "rf12.h"
#include "../common/event.h"
#include "../common/UART.h"
#include "ServMenu.h"

#define		MSK_ST_TOUR	0x80
#define		MSK_NEXT	0x20
#define		MSK_PREV	0x40
#define		MSK_CANCEL	0x10
#define		MSK_ST_BTN	0x08
#define		MSK_TURN_BTN	0x04

#define		SND_SHORT		0x00
#define		SND_LONG		0x0F
#define		SND_SHORT_SHORT	0x02
#define		SND_SHORT_LONG	0x1E



#define		_mS10		~(CPU_FREQ/256/100 - 1)  //Prescaller 256


#define		_SndOn		PORTC_Bit1 = 1; PORTD_Bit5 = 1; PORTD_Bit4 = 0
#define		_SndOff		PORTC_Bit1 = 0; PORTD_Bit5 = 0; PORTD_Bit4 = 1




typedef enum
{
  INIT_ST,
  IDLE_ST,
  LAUNCH_TIME_ST,
  READY_TIME_ST,
  TIME_OUT_START_ST,
  TOUR_ST,
  STOP_ST,
  CANCEL_ST
}DEV_STATE;


typedef enum
{
  UPDATE_DISP_TIME,
  UPDATE_DISP_LAP,
  TOUR_GO,
  OUT_OF_BASE,
  EN_ST_BTN
}FLAGS;

typedef enum
{
  GREEN_ON,
  RED_ON,
  BLINK,
  ALL_OFF
}LEDCTRL;


__eeprom __no_init uchar eMode;
__eeprom __no_init uchar eLastSec;
__eeprom __no_init uchar eLaunchTime;
__eeprom __no_init uchar eTimeFormat;


uchar volatile	Flags;
uchar		LapNum;			//number of passed lap
uint*		LapResult;		//pointer to current storage cell
uint		Results[10];	//table for pass results
uint		Result;			//current time of tour
uint		PressTime;
uint		ReadyTimer;
uchar		CurCode;		//for buttons handler
uchar volatile	ScanCode;		
uchar volatile	TmpCode;
uchar volatile	Delay1;			//variable for LCD & UART delay
uchar		StateDev;
uchar volatile	SndTime;
uchar volatile	Ring;
uchar volatile	Sound;
uchar		LastSecondSnd;	//var. Buzzer cnt to end of starting time
uchar 		LaunchTime;
uchar		TimeFormat;
uchar		LedState;
//char     NameBuf[16];

void InitCPU();
void InitTimers(void);
void SndOn(uchar mask);
void SndOnRing(uchar len);
void LedCtrl(LEDCTRL led);
void PrintTime(uint timer, uchar format);
void PrintTimeShort(uchar ten_min, uint timer);
void KeyHandler(void);
void UpdateDispLap(uchar num);
void UpdateDispTime(uint time);
void UpdatePredictTime(uchar num);



/************************************************************************/
/*	П Р Е Р Ы В А Н И Я						*/
/************************************************************************/

#pragma vector = TIMER1_OVF_vect
__interrupt  void TIMER1_OVF_interrupt(void)
{
  TCNT1 += _mS10;
  Result++;			//result. always increment
  if (Flags & (1 << TOUR_GO))
  {
    if (!(Result %10)) Flags |= (1 << UPDATE_DISP_TIME);
  }
  if (ReadyTimer)
  {
    ReadyTimer--;			//ready timer always decrement
    if (!(ReadyTimer % 100)) Flags |= (1 << UPDATE_DISP_TIME);
    else if ((LedState) && (!(ReadyTimer % 50))) PORTC |= LedState;
  }
  
  if(Delay1) --Delay1;
  
  //buttons handle
  if (!(PIND & 0x04)) 		//press turn button
  {
    if ((CurCode ^ TmpCode) & MSK_TURN_BTN)
    {
      CurCode |= MSK_TURN_BTN;
      ScanCode |= MSK_TURN_BTN;
    }
    else TmpCode |= MSK_TURN_BTN;
  }
  else {TmpCode &= ~MSK_TURN_BTN; CurCode &= ~MSK_TURN_BTN;}
  
  if (!(PIND & 0x08)) 		//press start button
  {
    if ((CurCode ^ TmpCode) & MSK_ST_BTN)
    {
      CurCode |= MSK_ST_BTN;
      ScanCode |= MSK_ST_BTN;
    }
    else TmpCode |= MSK_ST_BTN;
  }
  else {TmpCode &= ~MSK_ST_BTN; CurCode &= ~MSK_ST_BTN;}
  
  if (!(PINC & 0x10)) 		//press but K0 StartTour
  {
    if ((CurCode ^ TmpCode) & MSK_ST_TOUR)
    {
      CurCode |= MSK_ST_TOUR;
      ScanCode |= MSK_ST_TOUR;
    }
    else TmpCode |= MSK_ST_TOUR;
  }
  else {TmpCode &= ~MSK_ST_TOUR; CurCode &= ~MSK_ST_TOUR;}
  
  if (!(PINC & 0x20)) 		//press but K1 "+" Next
  {
    if ((CurCode ^ TmpCode) & MSK_NEXT)
    {
      CurCode |= MSK_NEXT;
      ScanCode |= MSK_NEXT;
    }
    else TmpCode |= MSK_NEXT;
  }
  else {TmpCode &= ~MSK_NEXT; CurCode &= ~MSK_NEXT;}
  
  if (!(PINC & 0x40)) 		//press but K2 Cancel
  {
    if ((CurCode ^ TmpCode) & MSK_CANCEL)
    {
      CurCode |= MSK_CANCEL;
      ScanCode |= MSK_CANCEL;
    }
    else TmpCode |= MSK_CANCEL;
  }
  else {TmpCode &= ~MSK_CANCEL; CurCode &= ~MSK_CANCEL;}
  
  if (!(PINC & 0x80)) 		//press but K3 "-" Prev
  {
    if ((CurCode ^ TmpCode) & MSK_PREV)
    {
      CurCode |= MSK_PREV;
      ScanCode |= MSK_PREV;
    }
    else TmpCode |= MSK_PREV;
  }
  else {TmpCode &= ~MSK_PREV; CurCode &= ~MSK_PREV;}
  
  //Sound controll
  if (SndTime) SndTime--;
  else if (Ring)
  {
    SndTime = 2;
    if (Ring & 0x01) {_SndOn;}
    else {_SndOff;}
    Ring--;
  }
  else if (Sound)
  {
    SndTime = 20;
    if (Sound & 0x01) {_SndOn;}
    else {_SndOff;}
    Sound >>= 1;
  }
  else {_SndOff;}
}


/************************************************************************/
/*	Init board */
/************************************************************************/
void InitCPU()
{
  PORTA = 0xFE;
  DDRA = 0xFE; //0xFE;
  PORTB = 0xFF;
  DDRB = 0xFF;
  PORTC = 0xF0;
  DDRC = 0x0F;
  PORTD = 0xFF;
  DDRD = 0xF3;
}

/*************************************************************************/
void InitTimers(void)
{
  TIMSK = (1<<TOIE1);
  TCCR1B = 0x04;    //Prescaller 4
  TCNT1 = _mS10;
  
  //  TCCR1A = 0x00;
  //  TCCR1B = 0x02;
}

/************************************************************************/
/* F U N C T I O N */
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

/************************************************************************/
void LedCtrl(LEDCTRL led)
{
  if (led == GREEN_ON)
  {
    PORTC_Bit3 = 1;
    PORTC_Bit2 = 0;
    LedState = PINC & 0x00;
  }
  else if (led == RED_ON)
  {
    PORTC_Bit3 = 0;
    PORTC_Bit2 = 1;
    LedState = PINC & 0x00;
  }
  else
  {
    if (led == BLINK) LedState = PINC & 0x0C;
    else LedState = PINC & 0x00;
    PORTC_Bit3 = 0;
    PORTC_Bit2 = 0;
  }
}

/************************************************************************/
void PrintTime(uint timer, uchar format)
{
  if (format)
  {
    putchar(timer / 6000 + 0x30); //minute
    timer %= 6000;
    putchar(':');
    putchar(timer / 1000 + 0x30);
    timer %= 1000;
    putchar(timer / 100 + 0x30);
    timer %= 100;
  }
  else
  {
    if (timer > 10000)
    {
      putchar(timer / 10000 + 0x30); //hundred of sec
      timer %= 10000;
    }
    else putchar(' ');
    putchar(timer / 1000 + 0x30);
    timer %= 1000;
    putchar(timer / 100 + 0x30);
    timer %= 100;
  }
  putchar('.');
  putchar(timer / 10 + 0x30);
  putchar(timer % 10 + 0x30);
}

/************************************************************************/
void PrintTimeShort(uchar ten_min, uint timer)
{
  putchar(ten_min + 0x30);
  putchar(timer / 6000 + 0x30);
  timer %= 6000;
  putchar(':');
  putchar(timer / 1000 + 0x30);
  timer %= 1000;
  putchar(timer / 100 + 0x30);
}


/************************************************************************/
void KeyHandler(void)
{
  
  if (!(ScanCode)) return;
  
  if (ScanCode & MSK_ST_TOUR)				//tour's begin
  {
    PostEvent(START_ROUND, 0, MAIN_DEV);
    ScanCode &= ~MSK_ST_TOUR;
  }
  
  if (ScanCode & MSK_NEXT)				//but +
  {
    PostEvent(NEXT, 0, MAIN_DEV);
    //    PostEvent(NEXT, 0, UART_COMP);
    ScanCode &= ~MSK_NEXT;
  }
  
  if (ScanCode & MSK_PREV)				//but -
  {
    PostEvent(PREV, 0, MAIN_DEV);
    ScanCode &= ~MSK_PREV;
  }
  
  if (ScanCode & MSK_CANCEL)				//but reset
  {
    PostEvent(CANCEL, 0, MAIN_DEV);
    //    PostEvent(CANCEL, 0, UART_COMP);
    ScanCode &= ~MSK_CANCEL;
  }
  
  if (ScanCode & MSK_ST_BTN)				//but start point
  {
    if (Flags & (1 << EN_ST_BTN)) PostEvent(TIME_STAMP, Result, MAIN_DEV);
    ScanCode &= ~MSK_ST_BTN;
  }
  
  if (ScanCode & MSK_TURN_BTN)				//but turn point
  {
    if (!(Flags & (1 << EN_ST_BTN))) PostEvent(TIME_STAMP, Result, MAIN_DEV);
    ScanCode &= ~MSK_TURN_BTN;
  }
}

/************************************************************************/
void UpdateDispLap(uchar num)
{
  if (num > 9) num = 0;
  ClrStrDisp(0);
  SetCursDisp(0,1);
  putchar((num + 1) / 10 + 0x30);
  putchar((num + 1) % 10 + 0x30);
  SetCursDisp(0,8);
  if (num == 0) PrintTime(Results[0], TimeFormat);
  else PrintTime((Results[num] - Results[num - 1]), TimeFormat);
}

/************************************************************************/
void UpdateDispTime(uint time)
{
  Flags &= ~(1 << UPDATE_DISP_TIME);
  SetCursDisp(1, 8);
  PrintTime(time, TimeFormat);		// print current time
}

/************************************************************************/
void UpdatePredictTime(uchar num)
{
  ulong tmp;
  
  if (num > 9) return;
  SetCursDisp(1, 0);
  tmp = Results[num];
  tmp *= 10;
  tmp /= (num + 1);
  PrintTime((uint)tmp, TimeFormat);
}

/************************************************************************/
/*		M A I N */
/************************************************************************/

void main(void)
{
  T_EVENT* p_event;
  uint speed, tmp;
  //  char* name_buf;
  
  InitCPU();
  InitTimers();
  InitEventList();
  //  InitUART(1152);
  //  _UART_RX_EN;
  _SEI();
  if (InitDisp() == 0) LedCtrl(RED_ON);
  
  if (!(PINC & 0x10)) 		//pressed but 3 StartTour
  {
    SndOn(SND_LONG);
    Service_Menu();
  }
  else SndOn(SND_SHORT_SHORT);
  Delay1 = 80;
  while (Delay1);
  LedCtrl(GREEN_ON);
  Delay1 = 150;
  while (Delay1);
  LedCtrl(RED_ON);
  Delay1 = 150;
  while (Delay1);
  StateDev = INIT_ST;
  LedCtrl(ALL_OFF);
  LastSecondSnd = eLastSec;
  LaunchTime = eLaunchTime;
  TimeFormat = eTimeFormat;
  StateDev = INIT_ST;
  LapNum = 0;
  ScanCode = 0;
  SndOn(SND_SHORT);
  for(;;)
  {
    KeyHandler();
    
    /*    p_event = GetPacket();
    if (p_event != NULL)
    PostEvent(p_event->cmd, p_event->param0, p_event->addr);*/
    
    p_event = GetEvent();
    if (p_event != NULL)
    {
      if ((p_event->addr > 0) && (p_event->addr < 5))
      {
        //if event not for main device - send it and mark as handled
        SendPacket(p_event);
        p_event = NULL;
      }
    }
    
    //Handle incoming event for main device and update display
    
    
    switch (StateDev)
    {
    case INIT_ST:				//init
      ClrAllDisp();
      WriteStr(" Система *F3F*");
      SetCursDisp(1, 6);
      WriteStr("Готов");
      Flags = (1 << EN_ST_BTN);
      LapNum = 0;
      LapResult = Results;
      memset(Results, 0, sizeof(Results));
      ReadyTimer = 0;
      LedCtrl(ALL_OFF);
      StateDev = IDLE_ST;
      break;
      
      /*      case WAIT_NAME_ST:
      if (ReadyTimer == 0)
      {
      StateDev = IDLE_ST;
      break;
    }
      if (p_event == NULL) break;
      if (p_event->cmd == LETTER)
      {
      *name_buf = (char)p_event->param0;
      name_buf++;
    }
        else if (p_event->cmd == END_CHAR)
      {
      StateDev = IDLE_ST;
      ClrStrDisp(0);
      SetCursDisp(0, 0);
      WriteStr((uchar*)NameBuf);
    }
      break;*/
      
    case IDLE_ST:				//ready to begin
      if (p_event == NULL) break;
      if (p_event->cmd == START_ROUND)	//event arrived
      {
        if (LaunchTime)
        {
          ClrAllDisp();
          WriteStr(" Взлет разрешен\n");
          SetCursDisp(1, 5);
          PrintTimeShort(0, ReadyTimer);
          _CLI();
          ReadyTimer = LaunchTime * 100;
          _SEI();
          StateDev = LAUNCH_TIME_ST;
          SndOn(SND_LONG);
          LedCtrl(GREEN_ON);
        }
        else //The Launch time == 0
        {
          ClrAllDisp();
          WriteStr("  Набор высоты\n");
          ReadyTimer = 3000; 		//30 sec because of F3F rule
          StateDev = READY_TIME_ST;
          SndOn(SND_LONG);
          LedCtrl(RED_ON);
          Flags |= (1 << UPDATE_DISP_TIME);
        }
      }
      break;
      
    case LAUNCH_TIME_ST:							//time to launch the model
      if (Flags & (1 << UPDATE_DISP_TIME))	//update ready timer
      {
        Flags &= ~(1 << UPDATE_DISP_TIME);
        SetCursDisp(1,5);
        PrintTimeShort(0, ReadyTimer);
        if (ReadyTimer <= (LastSecondSnd*100)) SndOn(SND_SHORT);	//last second (buzzer)
        if (ReadyTimer == 0)
        {
          //PostEvent(READY_TIME_OUT, 0, START_BTN);				//time expired. Round canceled
          StateDev = CANCEL_ST;
        }
        if ((LaunchTime * 100 - ReadyTimer) < 150) break;
      }
      if (p_event == NULL) break;				//no event
      if (p_event->cmd == START_ROUND)			//event arrived from start point, model in the air
      {
        ClrAllDisp();
        WriteStr("  Набор высоты\n");
        ReadyTimer = 3000; 		//30 sec because of F3F rule
        StateDev = READY_TIME_ST;
        SndOn(SND_LONG);
        LedCtrl(RED_ON);
        Flags |= (1 << UPDATE_DISP_TIME);
        break;
      }
      if (p_event->cmd == CANCEL) StateDev = CANCEL_ST;
      break;
      
    case READY_TIME_ST:							//time to rase altitude (F3F - 30 sec)
      if (Flags & (1 << UPDATE_DISP_TIME))	//update ready timer
      {
        Flags &= ~(1 << UPDATE_DISP_TIME);
        SetCursDisp(1,5);
        PrintTimeShort(0, ReadyTimer);
        if (ReadyTimer <= (LastSecondSnd*100)) SndOn(SND_SHORT);	//last second (buzzer)
        if (ReadyTimer == 0)
        {
          //PostEvent(READY_TIME_OUT, 0, START_BTN);				//time expired. autostart tour. send event to start point
          SndOn(SND_LONG);
          Flags |= (1 << EN_ST_BTN);
          ClrAllDisp();
          Flags |= ((1 << UPDATE_DISP_LAP) + (1 << UPDATE_DISP_TIME) + (1 << TOUR_GO));
          StateDev = TIME_OUT_START_ST;	//go to new state
          
          Result = 0;
        }
      }
      if (p_event == NULL) break;				//no event
      if (p_event->cmd == TIME_STAMP)			//event arrived from strart point
      {
        if (Flags & (1 << OUT_OF_BASE))	//was out of base - start the rase
        {
          ClrAllDisp();
          Flags |= ((1 << UPDATE_DISP_LAP) + (1 << UPDATE_DISP_TIME) + (1 << TOUR_GO));
          SndOn(SND_LONG);
          ReadyTimer = 150;				//1.5 sec no reaction on event
          StateDev = TOUR_ST;				//tour running
          Result = 0;
          speed = Result;
          //speed = p_event->param0;
          /*_CLI();
          Result = p_event->param0;
          _SEI();*/
          //PostEvent(SOUND, 2, TURN_BTN);
          Flags &= ~(1 << EN_ST_BTN);
        }
        else 		//out of base wait event from start point again
        {
          Flags |= (1 << OUT_OF_BASE);
          SndOn(SND_SHORT_SHORT);
        }
        break;
      }
      if (p_event->cmd == CANCEL) StateDev = CANCEL_ST;
      break;
      
    case TIME_OUT_START_ST:
      if (Flags & (1 << UPDATE_DISP_LAP))
      {
        Flags &= ~(1 << UPDATE_DISP_LAP);
        UpdateDispLap(LapNum);
        
      }
      if (Flags & (1 << UPDATE_DISP_TIME)) UpdateDispTime(Result);
      if (p_event == NULL) break;				//no event
      if (p_event->cmd == TIME_STAMP)			// event arrived from start point
      {
        if (Flags & ( 1 << OUT_OF_BASE))	//was out of base - start the race
        {
          SndOn(SND_SHORT);
          ReadyTimer = 150;				//1.5 sec - no reaction on points event
          StateDev = TOUR_ST;
          //speed = Result;
          speed = p_event->param0;
          //_CLI();
          //Result = p_event->param0;
          //_SEI();*/
          //            PostEvent(SOUND, 2, TURN_BTN);
          Flags &= ~(1 << EN_ST_BTN);
        }
        else
        {
          Flags |= (1 << OUT_OF_BASE);	//out of base wait event from start point again
          SndOn(SND_SHORT_SHORT);
        }
        break;
      }
      if (p_event->cmd == CANCEL) StateDev = CANCEL_ST;
      break;
      
    case TOUR_ST:								//Tour is in process
      if (Flags & (1 << UPDATE_DISP_LAP))
      {
        if (LapNum == 1)					// first pass - print the speed
        {
          SetCursDisp(0,0);
          WriteStr("Скорость ");
          tmp = *(LapResult - 1) - speed;
          speed = 36000 / tmp;
          putchar(speed / 100 + 0x30);
          speed %= 100;
          putchar(speed / 10 + 0x30);
          putchar(speed % 10 + 0x30);
          WriteStr("км/ч");
        }
        else
        {
          UpdateDispLap(LapNum - 1);
          UpdatePredictTime(LapNum - 1);
        }
        Flags &= ~(1 << UPDATE_DISP_LAP);
      }
      if (Flags & (1 << UPDATE_DISP_TIME)) UpdateDispTime(Result);
      if (p_event == NULL) break;
      if ((ReadyTimer == 0) && (p_event->cmd == TIME_STAMP))			//event from turn point
      {
        ReadyTimer = 150;					//sensless time
        if (LapNum >= 9)					//if it was the last pass
        {
          *LapResult = p_event->param0;		//save time of pass
          Flags &= ~(1 << TOUR_GO);		//clear tour flag and issue event of finish
          SndOnRing(40);
          StateDev = STOP_ST;
          ClrAllDisp();
          WriteStr("Скорость ");
          speed = 36000 / tmp;
          putchar(speed / 100 + 0x30);
          speed %= 100;
          putchar(speed / 10 + 0x30);
          putchar(speed % 10 + 0x30);
          WriteStr("км/ч");
          SetCursDisp(1, 0);
          WriteStr("Рез-тат  ");
          UpdateDispTime(*LapResult);
        }
        else					//not last pass
        {
          Flags |= 1 << UPDATE_DISP_LAP;
          if (LapNum & 0x01) 			//turn on "start point" - sound for "turn point"
          {
            
            *LapResult = p_event->param0;	//save time of pass
            _CLI();
            Result = p_event->param0;		//update sown time
            _SEI();
            //              PostEvent(SOUND, 1, TURN_BTN);
            Flags &= ~(1 << EN_ST_BTN);
            
          }
          else				//turn on "turn point" - sound for "start point"
          {
            *LapResult = Result;		//save time of pass from main device. turn button result omited
            //PostEvent(SOUND, 1, START_BTN);
            Flags |= (1 << EN_ST_BTN);
          }
          if (LapNum == 8) SndOn(SND_SHORT_LONG);		//penultimate pass
          else SndOn(SND_SHORT);
          LapNum++;
          LapResult++;
        }
        break;
      }
      if (p_event->cmd == CANCEL) StateDev = CANCEL_ST;
      break;
      
    case STOP_ST:							//FINISH state
      if (Flags & (1 << UPDATE_DISP_LAP))
      {
        UpdateDispLap(LapNum);
        Flags &= ~(1 << UPDATE_DISP_LAP);
      }
      if (p_event == NULL) break;
      if (p_event->cmd == PREV)			//button "-"
      {
        if (LapNum) LapNum--;
        else LapNum = 9;
        Flags |= (1 << UPDATE_DISP_LAP);
      }
      if (p_event->cmd == NEXT)		//button "+"
      {
        if (LapNum != 9) LapNum++;
        else LapNum = 0;
        Flags |= (1 << UPDATE_DISP_LAP);
      }
      if (p_event->cmd == CANCEL) StateDev = INIT_ST;
      break;
      
    case CANCEL_ST:
      ClrAllDisp();
      WriteStr("     Отмена\n");
      Delay1 = 150;
      SndOn(SND_LONG);
      while (Delay1);
      Delay1 = 150;
      SndOn(SND_LONG);
      while (Delay1);
      StateDev = INIT_ST;
      break;
    default:
      break;
    }
  }
}

