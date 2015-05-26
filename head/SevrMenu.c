#include "cpu.h"
#include "ind2_16.h"
#include "../common/event.h"
#include "../common/UART.h"


#define MAX_MENU_ITEM	(sizeof(servmenu)/sizeof(TSERVMENU))

typedef struct
{
  const char* menustr;     		// Строка
  void   (*func)(void);	  		// Указатель на функцию
} TSERVMENU;

//from main.c
extern __eeprom __no_init uchar eMode;
extern __eeprom __no_init uchar eLastSec;
extern __eeprom __no_init uchar eLaunchTime;
void KeyHandler(void);

void Mode_Menu(void);
void LastSec_Menu(void);
void LaunchTime_Menu(void)



TSERVMENU servmenu[] = {
  {"Режим     ", Mode_Menu},
  {"Сигнал   ", LastSec_Menu},
  {"Время на старт  ", LaunchTime_Menu}
};

/************************************************************************/
/*	О Р Г А Н И З А Ц И Я   С Л У Ж Е Б Н О Г О    М Е Н Ю		*/
/************************************************************************/
void Service_Menu(void)
{
  uchar  MenuItem;
  T_EVENT *p_event;

  MenuItem = 0;
  ClrAllDisp();
  WriteStr((uchar*)servmenu[0].menustr);
  while (!(PINC & 0x40)); 		//pressed but 3 StartTour
  KeyHandler();
  p_event = GetEvent();
  p_event = NULL;


  for (;;)
  {
    KeyHandler();
    p_event = GetEvent();
    if (p_event != NULL)
    {
      if (p_event->addr !=  5)
      {
          //if event not for main device - mark it as handled
          p_event = NULL;
          continue;
      }
      if (p_event->cmd == PREV)			//button "-"
      {
        p_event = NULL;
        if (MenuItem == 0) MenuItem = MAX_MENU_ITEM - 1;
        else MenuItem--;
        SetCursDisp(0, 0);
        WriteStr((uchar*)servmenu[MenuItem].menustr);
      }
      else if (p_event->cmd == NEXT)		//button "+"
      {
        p_event = NULL;
        if (MenuItem >= (MAX_MENU_ITEM - 1)) MenuItem = 0;
        else MenuItem++;
        SetCursDisp(0, 0);
        WriteStr((uchar*)servmenu[MenuItem].menustr);
      }
      else if (p_event->cmd == START_ROUND)	//enter
      {
        p_event = NULL;
        SetCursDisp(0,15);
        putchar('*');
        servmenu[MenuItem].func();
        ClrAllDisp();
        WriteStr((uchar*)servmenu[MenuItem].menustr);
      }
      else if (p_event->cmd == CANCEL)
      {
        p_event = NULL;
        return;
      }
      else p_event = NULL;
    }
  }
}

/************************************************************************/
void Mode_Menu(void)
{
  uchar  mode = eMode;
  uchar prn_flag = 1;
  T_EVENT *p_event;


  if (mode > 2) mode = 0;
  for (;;)
  {
    KeyHandler();
    p_event = GetEvent();
    if (p_event != NULL)
    {
      if (p_event->addr !=  5)
      {
          //if event not for main device - mark it as handled
          p_event = NULL;
          continue;
      }
      if (p_event->cmd == PREV)			//button "-"
      {
        p_event = NULL;
        if (mode != 0) mode--;
        prn_flag = 1;
      }
      else if (p_event->cmd == NEXT)		//button "+"
      {
        p_event = NULL;
        if (mode < 2) mode++;
        prn_flag = 1;
      }
      else if (p_event->cmd == CANCEL)
      {
        p_event = NULL;
        eMode = mode;
        return;
      }
      else p_event = NULL;
    }
    if (prn_flag)
    {
      prn_flag = 0;
      ClrStrDisp(1);
      SetCursDisp(1,0);
      switch(mode)
      {
        case 0:
          WriteStr("Соревнования");
          break;
        case 1:
          WriteStr("Тренер 1 вешка");
          break;
        case 2:
          WriteStr("Тренер 2 вешки");
          break;
      }
    }

  }
}

/************************************************************************/
void LastSec_Menu(void)
{
  uchar  last_sec = eLastSec;
  uchar prn_flag = 1;
  T_EVENT *p_event;

  if (last_sec > 20) last_sec = 10;
  WriteStr("Кол-во пиков    ");
  for (;;)
  {
    KeyHandler();
    p_event = GetEvent();
    if (p_event != NULL)
    {
      if (p_event->addr !=  5)
      {
          //if event not for main device - mark it as handled
          p_event = NULL;
          continue;
      }
      if (p_event->cmd == PREV)			//button "-"
      {
        p_event = NULL;
        if (last_sec != 0) last_sec--;
        prn_flag = 1;
      }
      else if (p_event->cmd == NEXT)		//button "+"
      {
        p_event = NULL;
        if (last_sec < 20) last_sec++;
        prn_flag = 1;
      }
      else if (p_event->cmd == CANCEL)
      {
        p_event = NULL;
        eLastSec = last_sec;
        return;
      }
      else p_event = NULL;
    }
    if (prn_flag)
    {
      prn_flag = 0;
      SetCursDisp(1,13);
      if ((last_sec / 10) != 0) putchar(last_sec/10 + 0x30);
      else putchar(' ');
      putchar(last_sec%10 + 0x30);
    }
  }
}

/************************************************************************/
void LaunchTime_Menu(void)
{
  uchar  launch_time = eLaunchTime;
  uchar prn_flag = 1;
  T_EVENT *p_event;

  if (launch_time > 20) launch_time = 10;
  WriteStr("Время на запуск ");
  for (;;)
  {
    KeyHandler();
    p_event = GetEvent();
    if (p_event != NULL)
    {
      if (p_event->addr !=  5)
      {
          //if event not for main device - mark it as handled
          p_event = NULL;
          continue;
      }
      if (p_event->cmd == PREV)			//button "-"
      {
        p_event = NULL;
        if (launch_time != 0) launch_time--;
        prn_flag = 1;
      }
      else if (p_event->cmd == NEXT)		//button "+"
      {
        p_event = NULL;
        if (launch_time < 20) launch_time++;
        prn_flag = 1;
      }
      else if (p_event->cmd == CANCEL)
      {
        p_event = NULL;
        eLaunchTime = launch_time;
        return;
      }
      else p_event = NULL;
    }
    if (prn_flag)
    {
      prn_flag = 0;
      SetCursDisp(1,13);
      if ((launch_time / 10) != 0) putchar(launch_time/10 + 0x30);
      else putchar(' ');
      putchar(launch_time%10 + 0x30);
    }
  }
}
