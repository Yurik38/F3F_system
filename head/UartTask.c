#include "cpu.h"
#include "main.h"
#include "UartTask.h"
#include "../common/event.h"
#include "../common/UART.h"

#define DATA_BUF_SIZE	18

uchar	DataBuffer[DATA_BUF_SIZE];

static uchar parse_state = 0;
static uchar crc, cnt, cur_ID;
static uchar *buf;

uchar GetPacket(void)
{
	uchar rx_byte;

	if (GetByte(&rx_byte))
	{
		Delay1 = 5;
		switch (parse_state)
		{
			case 0:
				if (rx_byte == 0x7E)		  //начало пакета
				{
					parse_state = 1;
					crc = 0;
					buf = DataBuffer;
				}
				break;

			case 1://rx size
				cnt = rx_byte;
				crc += rx_byte;
				parse_state++;
				break;

			case 2://rx cmd and packet
				buf = rx_byte;
				buf++;
				crc += rx_byte;
				if (cnt) cnt--;
				else parse_state++;
				break;
				
			case 3://rx crc and check
				parse_state = 0;
				if (crc != rx_byte) break;			//CRC mismatch
				return 1;

			default:
				parse_state = 0;
				break;
		}
	}
	else
	{
		if ((parse_state) && (Delay1 == 0))//if time expired reset parser
			parse_state = 0;
	}
	return 0;
}

/************************************************************************/
void SendPacket(T_EVENT* event)
{
	uchar i;
	uchar crc;
	uchar *ptr = (uchar*)event->param0;

	Delay1 = 5;

	while ((UARTBusyFlag) || (Delay1));
	tx_buffer[0] = 0x7E;
	if (event->cmd == CMD_TIME_STAMP) tx_buffer[1] = sizeof(Result);
	else if (event->cmd == CMD_WEATHER_STAMP) tx_buffer[1] = sizeof(Weather);
	else tx_buffer[1] = 0;
	crc = event->cmd + tx_buffer[1];
	tx_buffer[2] = event->cmd;

	for (i = 3; i < tx_buffer[1] + 3; i++)
	{
		tx_buffer[i] = *ptr;
		crc += *ptr;
		ptr++;
	}
	tx_buffer[i] = crc;
	i++;
	TxBuffer(tx_buffer, i);
}

void UartTask(void)
{
	T_COMMAND cmd;
	
	if (GetPacket() != 0)
	{
		cmd = (T_COMMAND)DataBuffer[0];
		switch(cmd)
	}
}
