#include "cpu.h"
#include "ind2_16.h"
#include "../common/event.h"
#include "../common/UART.h"


#define MAX_MENU_ITEM	(sizeof(servmenu)/sizeof(TSERVMENU))

typedef struct
{
  flash char* menustr;     		// ������
  void   (*func)(void);	  		// ��������� �� �������
} TSERVMENU;


void Mode_Menu(void);
void LastSec_Menu(void);



TSERVMENU servmenu[] = {
  {"�����     ", Mode_Menu},
  {"������   ", LastSecSnd_Menu},
};

/************************************************************************/
/*	� � � � � � � � � � �   � � � � � � � � � �    � � � �		*/
/************************************************************************/
void Service_Menu(void)
{

uchar  MenuItem;
uchar  LastSec = eLastSec;
uchar  Mode = eMode;

  MenuItem = 0;
  ClrAllDisp();
  WriteStr(servmenu[0].menustr);
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
        if (MenuItem == 0) MenuItem = MAX_MENU_ITEM;
        else MenuItem--;
        WriteStr(servmenu[MenuItem].menustr);
      }
      else if (p_event->cmd == NEXT)		//button "+"
      {
        p_event = NULL;
        if (MenuItem >= MAX_MENU_ITEM) MenuItem = 0;
        else MenuItem++;
        WriteStr(servmenu[MenuItem].menustr);
      }
      else if (p_event->cmd == START_ROUND)	//enter
      {
        p_event = NULL;
        SetCursDisp(0,15);
        put_char('*');
        servmenu[MenuItem].func();
        ClrAllDisp();
        WriteStr(servmenu[MenuItem].menustr);
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
      ClrStr(1);
      SetCursDisp(1,0);
      switch(Mode)
      {
        case 0:
          WriteStr("������������");
          break;
        case 1:
          WriteStr("������ 1 �����");
          break;
        case 2:
          WriteStr("������ 2 �����");
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

  if (last_sec > 20) last_sec = 10;
  WriteStr("���-�� �����    ");
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
      if ((last_sec / 10) != 0) put_char(last_sec/10 + 0x30);
      else put_char(' ');
      put_char(last_sec%10 + 0x30);
    }
  }
}
