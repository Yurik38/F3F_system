/*
Schematic diagram 	tablo_smpl.sch
No radio buttons, hardcore and wires only

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
#include "ServMenu.h"
#include "../common/event.h"
#include "../common/UART.h"

#define		MSK_ST_TOUR	0x80
#define		MSK_NEXT	0x20
#define		MSK_PREV	0x40
#define		MSK_CANCEL	0x10

#define		SND_SHORT		0x00
#define		SND_LONG		0x1F
#define		SND_SHORT_SHORT	0x02
#define		SND_SHORT_LONG	0x3E

#define		_mS10		~(432 - 1)  //Prescaller 256

#define		_SndOn		PORTD |= 0x30; PORTC &= ~0x02
#define		_SndOff		PORTD &= ~0x30; PORTC |= 0x02

typedef enum
{
  INIT_ST,
  IDLE_ST,
  LUNCH_TIME_ST,
  READY_TIME_ST,
  TIME_OUT_START_ST,
  TOUR_ST,
  STOP_ST
}DEV_STATE;


typedef enum
{
  UPDATE_DISP_TIME,
  UPDATE_DISP_LAP,
  TOUR_GO,
  OUT_OF_BASE,
  EN_ST_BTN
}FLAGS;

__eeprom __no_init uchar eMode;
__eeprom __no_init uchar eLastSec;
__eeprom __no_init uchar eLaunchTime;


uchar volatile	Flags;
uchar			LapNum;			//number of passed lap
uint*			LapResult;		//pointer to current storage cell
uint			Results[10];	//table for pass results
uint			Result;			//current time of tour
uint			PressTime;
uint			ReadyTimer;
uint			LaunchTime;
uchar			CurCode;		//for buttons handler
uchar volatile	ScanCode;		
uchar volatile	TmpCode;
uchar volatile	Delay1;			//variable for LCD & UART delay
uchar			StateDev;
uchar volatile	SndTime;
uchar volatile	Ring;
uchar volatile	Sound;

uchar			LastSecondSnd;	//var. Buzzer cnt to end of starting time
/************************************************************************/
/*	� � � � � � � � � �						*/
/************************************************************************/

#pragma vector = TIMER1_OVF_vect
__interrupt  void TIMER1_OVF_interrupt(void)
{
  uchar i;
  uchar* p;

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
  }

  if(Delay1) --Delay1;

  //buttons handle
  if (!(PINC & 0x20)) 		//press but 2 Cancel
  {
    if ((CurCode ^ TmpCode) & MSK_CANCEL)
    {
      //      _SndOnShort;
      CurCode |= MSK_CANCEL;
      ScanCode |= MSK_CANCEL;
    }
    else TmpCode |= MSK_CANCEL;
  }
  else {TmpCode &= ~MSK_CANCEL; CurCode &= ~MSK_CANCEL;}

  if (!(PINC & 0x10)) 		//press but  4 Next
  {
    if ((CurCode ^ TmpCode) & MSK_NEXT)
    {
      //      _SndOnShort;
      CurCode |= MSK_NEXT;
      ScanCode |= MSK_NEXT;
    }
    else TmpCode |= MSK_NEXT;
  }
  else {TmpCode &= ~MSK_NEXT; CurCode &= ~MSK_NEXT;}

  if (!(PINC & 0x80)) 		//press but 1 Prev
  {
    if ((CurCode ^ TmpCode) & MSK_PREV)
    {
      //      _SndOnShort;
      CurCode |= MSK_PREV;
      ScanCode |= MSK_PREV;
    }
    else TmpCode |= MSK_PREV;
  }
  else {TmpCode &= ~MSK_PREV; CurCode &= ~MSK_PREV;}

  if (!(PINC & 0x40)) 		//press but 3 StartTour
  {
    if ((CurCode ^ TmpCode) & MSK_ST_TOUR)
    {
      //      _SndOnShort;
      CurCode |= MSK_ST_TOUR;
      ScanCode |= MSK_ST_TOUR;
    }
    else TmpCode |= MSK_ST_TOUR;
  }
  else {TmpCode &= ~MSK_ST_TOUR; CurCode &= ~MSK_ST_TOUR;}

  if (!(PIND & 0x08)) 		//press turn button
  {
    if ((CurCode ^ TmpCode) & MSK_TURN_BTN)
    {
      CurCode |= MSK_TURN_BTN;
      ScanCode |= MSK_TURN_BTN;
    }
    else TmpCode |= MSK_TURN_BTN;
  }
  else {TmpCode &= ~MSK_TURN_BTN; CurCode &= ~MSK_TURN_BTN;}

  if (!(PIND & 0x04)) 		//press start button
  {
    if ((CurCode ^ TmpCode) & MSK_ST_BTN)
    {
      CurCode |= MSK_ST_BTN;
      ScanCode |= MSK_ST_BTN;
    }
    else TmpCode |= MSK_ST_BTN;
  }
  else {TmpCode &= ~MSK_ST_BTN; CurCode &= ~MSK_ST_BTN;}

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
void InitCPU(void)
{
  PORTA = 0xFE;
  DDRA = 0xFE; //0xFE;
  PORTB = 0xFF;
  DDRB = 0xFB;
  PORTC = 0xFF;
  DDRC = 0x0F;
  PORTD = 0xFF;
  DDRD = 0xFF;
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
inline void SndOn(uchar mask)
{
  _CLI();
  Sound = mask;
  _SndOn;
  SndTime = 20;
  _SEI();
}

/************************************************************************/
inline void SndOnRing(uchar len)
{
  _CLI();
  _SndOn;
  SndTime = 2;
  Ring = len;
  _SEI();
}

/************************************************************************/
void Light(uchar color);
{
  if (color == GREEN) 
  {
    PORTC_Bit3 = 1;
	PORTC_Bit2 = 0;
  }
  else if (color == RED) 
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

/************************************************************************/
void PrintTime(uchar ten_min, uint timer)
{
  putchar(ten_min + 0x30);
  putchar(timer / 6000 + 0x30);
  timer %= 6000;
  putchar(':');
  putchar(timer / 1000 + 0x30);
  timer %= 1000;
  putchar(timer / 100 + 0x30);
  putchar('.');
  timer %= 100;
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
	if (Type)								//Wireless buttons
	{
      PostEvent(START_ROUND, 0, TURN_BTN);
      PostEvent(START_ROUND, 0, START_BTN);
	}
    ScanCode &= ~MSK_ST_TOUR;
  }

  if (ScanCode & MSK_NEXT)				//but +
  {
    PostEvent(NEXT, 0, MAIN_DEV);
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
	if (Type)
	{
      PostEvent(CANCEL, 0, TURN_BTN);
      PostEvent(CANCEL, 0, START_BTN);
	}
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
  Flags &= ~(1 << UPDATE_DISP_LAP);
  if (num > 9) num = 0;
  ClrStrDisp(0);
  SetCursDisp(0,1);
  putchar((num + 1) / 10 + 0x30);
  putchar((num + 1) % 10 + 0x30);
  SetCursDisp(0,8);
  if (num == 0) PrintTime(0, Results[0]);
  else PrintTime(0, (Results[num] - Results[num - 1]));
}

/************************************************************************/
inline void UpdateDispTime(uint time)
{
  Flags &= ~(1 << UPDATE_DISP_TIME);
  SetCursDisp(1, 8);
  PrintTime(0, time);		// print current time
}


/************************************************************************/
/*		M A I N */
/************************************************************************/

void main(void)
{

  //  uint tmp_param;
  T_EVENT* p_event;

  uint speed, tmp;

  InitCPU();
  InitTimers();
  InitEventList();
  InitUART(1152);
  //  _UART_RX_EN;
  _SEI();
  if (InitDisp() == 0) _LedOn(0);
  if (!(PINC & 0x40)) 		//pressed but 3 StartTour
  {
    SndOn(SND_LONG);
    Service_Menu();
  }
  else SndOn(SND_SHORT_SHORT);
  _LedOffAll;

  StateDev = INIT_ST;
  LapNum = 0;
  ScanCode = 0;
  LastSecondSnd = eLastSec;
  LunchTime = 3000;

  if (eMode == 1) goto training_1;
  else if (eMode == 2) goto training_2;

  for(;;)
  {
    KeyHandler();
	

	//incoming events from RS232
    p_event = GetPacket();
    if (p_event != NULL)
	  PostEvent(p_event->cmd, p_event->param0, p_event->addr); //Place event into queue

	//Extract event from queue
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
        WriteStr(" ������� *F3F*");
        SetCursDisp(1, 0);
        WriteStr(" ����������\n");
        StateDev = IDLE_ST;
        Flags = 0;
        LapNum = 0;
        LapResult = Results;
        memset(Results, 0, sizeof(Results));
        ReadyTimer = 0;
        break;

      case IDLE_ST:				//ready to begin
        if (p_event == NULL) break;
        if (p_event->cmd == START_ROUND)	//event arrived
        {
          _CLI();
          ReadyTimer = LaunchTime;					
          _SEI();
          ClrAllDisp();
          WriteStr("���� �������� ");
		  Light(GREEN);
          SetCursDisp(1, 1);
          PrintTimeShort(0, ReadyTimer);
          SndOn(SND_LONG);
          StateDev = LUNCH_TIME_ST;
        }
        break;

      case LUNCH_TIME_ST:							//time to launch
        if (Flags & (1 << UPDATE_DISP_TIME))	//update ready timer
        {
          Flags &= ~(1 << UPDATE_DISP_TIME);
          SetCursDisp(1,1);
          PrintTimeShort(0, ReadyTimer);
          if (ReadyTimer <= (LastSecondSnd*100)) SndOn(SND_SHORT);	//last second (buzzer)
          if (ReadyTimer == 0)
          {
            if (Type) PostEvent(READY_TIME_OUT, 0, START_BTN);				//time expired. auto-start tour. send event to start point
            ClrAllDisp();
            Flags |= ((1 << UPDATE_DISP_LAP) + (1 << UPDATE_DISP_TIME) + (1 << TOUR_GO));
            StateDev = TIME_OUT_START_ST;	//go to new state
            Result = 0;
          }
        }
        if (p_event == NULL) break;				//no event
        if (p_event->cmd == TIME_STAMP)			//event arrived from strart point
        {
          if (Flags & ( 1 << OUT_OF_BASE))	//was out of base - start the rase
          {
            ClrAllDisp();
            Flags |= ((1 << UPDATE_DISP_LAP) + (1 << UPDATE_DISP_TIME) + (1 << TOUR_GO));
            SndOn(SND_LONG);
            ReadyTimer = 150;				//1.5 sec no reaction on event
            StateDev = TOUR_ST;				//tour running
            //Result = 0;
            //speed = Result;
            speed = p_event->param0;
            _CLI();
            Result = p_event->param0;
            _SEI();
            if (Type) PostEvent(SOUND, 2, TURN_BTN);
          }
          else 		//out of base wait event from start point again
          {
            Flags |= (1 << OUT_OF_BASE);
            SndOn(SND_SHORT_SHORT);
          }
          break;
        }
        if (p_event->cmd == CANCEL) StateDev = INIT_ST;
        break;
		

      case READY_TIME_ST:							//time to raise altitude (F3F - 30 sec)
        if (Flags & (1 << UPDATE_DISP_TIME))	//update ready timer
        {
          Flags &= ~(1 << UPDATE_DISP_TIME);
          SetCursDisp(1,1);
          PrintTimeShort(0, ReadyTimer);
          if (ReadyTimer <= (LastSecondSnd*100)) SndOn(SND_SHORT);	//last second (buzzer)
          if (ReadyTimer == 0)
          {
            if (Type) PostEvent(READY_TIME_OUT, 0, START_BTN);				//time expired. auto-start tour. send event to start point
            ClrAllDisp();
            Flags |= ((1 << UPDATE_DISP_LAP) + (1 << UPDATE_DISP_TIME) + (1 << TOUR_GO));
            StateDev = TIME_OUT_START_ST;	//go to new state
            Result = 0;
          }
        }
        if (p_event == NULL) break;				//no event
        if (p_event->cmd == TIME_STAMP)			//event arrived from strart point
        {
          if (Flags & ( 1 << OUT_OF_BASE))	//was out of base - start the rase
          {
            ClrAllDisp();
            Flags |= ((1 << UPDATE_DISP_LAP) + (1 << UPDATE_DISP_TIME) + (1 << TOUR_GO));
            SndOn(SND_LONG);
            ReadyTimer = 150;				//1.5 sec no reaction on event
            StateDev = TOUR_ST;				//tour running
            //Result = 0;
            //speed = Result;
            speed = p_event->param0;
            _CLI();
            Result = p_event->param0;
            _SEI();
            if (Type) PostEvent(SOUND, 2, TURN_BTN);
          }
          else 		//out of base wait event from start point again
          {
            Flags |= (1 << OUT_OF_BASE);
            SndOn(SND_SHORT_SHORT);
          }
          break;
        }
        if (p_event->cmd == CANCEL) StateDev = INIT_ST;
        break;

      case TIME_OUT_START_ST:
        if (Flags & (1 << UPDATE_DISP_LAP)) UpdateDispLap(LapNum);
        if (Flags & (1 << UPDATE_DISP_TIME)) UpdateDispTime(Result);
        if (p_event == NULL) break;				//no event
        if (p_event->cmd == TIME_STAMP)			// event arrived from start point
        {
          if (Flags & ( 1 << OUT_OF_BASE))	//was out of base - start the rase
          {
            SndOn(SND_LONG);
            ReadyTimer = 150;				//1.5 sec - no reaction on points event
            StateDev = TOUR_ST;
            //speed = Result;
            speed = p_event->param0;
            //_CLI();
            //Result = p_event->param0;
            //_SEI();*/
            if (Type) PostEvent(SOUND, 2, TURN_BTN);
          }
          else
          {
            Flags |= (1 << OUT_OF_BASE);	//out of base wait event from start point again
            SndOn(SND_SHORT_SHORT);
          }
          break;
        }
        if (p_event->cmd == CANCEL) StateDev = INIT_ST;
        break;

      case TOUR_ST:								//Tour is in process
        if (Flags & (1 << UPDATE_DISP_LAP))
        {
          if (LapNum == 1)					// first pass - print the speed
          {
            SetCursDisp(0,0);
            WriteStr("�������� ");
            tmp = *(LapResult - 1) - speed;
            speed = 36000 / tmp;
            putchar(speed / 100 + 0x30);
            speed %= 100;
            putchar(speed / 10 + 0x30);
            putchar(speed % 10 + 0x30);
            WriteStr("��/�");
            Flags &= ~(1 << UPDATE_DISP_LAP);
          }
          else UpdateDispLap(LapNum - 1);
        }
        if (Flags & (1 << UPDATE_DISP_TIME)) UpdateDispTime(Result);
        if (p_event == NULL) break;
        if (p_event->cmd == TIME_STAMP)			//event from turn point
        {
          Flags |= 1 << UPDATE_DISP_LAP;
          if (LapNum >= 9)					//if it was the last pass
          {
            *LapResult = p_event->param0;		//save time of pass
            Flags &= ~(1 << TOUR_GO);		//clear tour flag and issue event of finish
            //PostEvent(STOP, 0, MAIN_DEV);
            if (Type) 
			{
			  PostEvent(CANCEL, 0, START_BTN);
              PostEvent(SOUND, 3, START_BTN);
              PostEvent(CANCEL, 0, TURN_BTN);
              PostEvent(SOUND, 3, TURN_BTN);
            }
			SndOnRing(40);
            StateDev = STOP_ST;
            UpdateDispTime(*LapResult);
          }
          else					//not last pass
          {
            if (LapNum & 0x01) 			//turn on "start point" - sound for "turn point"
            {

              *LapResult = p_event->param0;	//save time of pass
              _CLI();
              Result = p_event->param0;		//update sown time
              _SEI();
              if (Type) PostEvent(SOUND, 1, TURN_BTN);
            }
            else				//turn on "turn point" - sound for "start point"
            {
              *LapResult = Result;		//save time of pass from main device. turn button result omited
              if (Type) PostEvent(SOUND, 1, START_BTN);
            }
            if (LapNum == 8) SndOn(SND_SHORT_LONG);		//penultimate pass
            else SndOn(SND_SHORT);
            LapNum++;
            LapResult++;
          }
          break;
        }
        if (p_event->cmd == CANCEL) StateDev = INIT_ST;
        break;

      case STOP_ST:							//FINISH state
        if (Flags & (1 << UPDATE_DISP_LAP)) UpdateDispLap(LapNum);
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
      default:
        break;
    }
  }

training_1:
  for(;;)
  {
    KeyHandler();

    p_event = GetPacket();
    if (p_event != NULL)
      PostEvent(p_event->cmd, p_event->param0, p_event->addr);

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
        WriteStr("������ 1 �����");
        SetCursDisp(1, 0);
        WriteStr(" ����� ������� 0");
        StateDev = CHECK_START_ST;
        Flags = 0;
        ReadyTimer = 0;
        break;

      case CHECK_START_ST:
        if (ReadyTimer == 0)
        {
          ReadyTimer = 250;
          PostEvent(BAT_VOLT_Q, 0, START_BTN);
        }
        if (p_event == NULL) break;
        if (p_event->cmd == VOLTAGE)	//answer from point
        {
          StateDev = TOUR_ST;
          SetCursDisp(1, 15);
          putchar('1');	
          ReadyTimer = 0;
        }
        break;

      case TOUR_ST:								//Tour is in process
        if (p_event == NULL) break;
        if (p_event->cmd == TIME_STAMP)			//event from turn point
        {
          SndOn(SND_SHORT);
        }
        break;

      default:
        break;
    }
  }

training_2:
  for(;;)
  {
    KeyHandler();

    p_event = GetPacket();
    if (p_event != NULL)
      PostEvent(p_event->cmd, p_event->param0, p_event->addr);

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
        WriteStr("������ 2 �����");
        SetCursDisp(1, 0);
        WriteStr(" ����� ������� 0");
        StateDev = CHECK_START_ST;
        Flags = 0;
        LapNum = 0;
        ReadyTimer = 0;
        break;

      case CHECK_START_ST:
        if (ReadyTimer == 0)
        {
          ReadyTimer = 250;
          PostEvent(BAT_VOLT_Q, 0, START_BTN);
        }
        if (p_event == NULL) break;
        if (p_event->cmd == VOLTAGE)	//answer from point
        {
          StateDev = CHECK_TURN_ST;
          SetCursDisp(1, 15);
          putchar('1');	
          ReadyTimer = 0;
        }
        break;

      case CHECK_TURN_ST:
        if (ReadyTimer == 0)
        {
          ReadyTimer = 250;
          PostEvent(BAT_VOLT_Q, 0, TURN_BTN);
        }
        if (p_event == NULL) break;
        if (p_event->cmd == VOLTAGE)	//answer from point
        {
          StateDev = TOUR_ST;
          SetCursDisp(1, 15);
          putchar('2');	
        }
        break;

      case TOUR_ST:								//Tour is in process
        if (p_event == NULL) break;
        if (p_event->cmd == TIME_STAMP)			//event from turn point
        {
          if (LapNum & 0x01) 			//turn on "start point" - sound for "turn point"
          {
            PostEvent(SOUND, 1, TURN_BTN);
          }
          else				//turn on "turn point" - sound for "start point"
          {
            PostEvent(SOUND, 1, START_BTN);
          }
          SndOn(SND_SHORT);
          LapNum++;
        }
        break;

      default:
        break;
    }
  }
}


