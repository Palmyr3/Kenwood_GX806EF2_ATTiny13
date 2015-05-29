#define CLOCK 9600000
#include <inavr.h>

__regvar __no_init unsigned char mode @15;  // ����� ������, 0: �� ������������� i2c
__regvar __no_init unsigned char xxx @14;   // ��������� ���������� ��� ������ START ������������������

unsigned char check1(void)
{
  GIFR|=(1<<INTF0);
  while (!(GIFR&(1<<INTF0)));
  GIFR|=(1<<INTF0);
  if ((PINB&(1<<PB0))==0x00) {xxx=PINB; return 1;}
  return 0;
}

unsigned char check0(void)
{
  GIFR|=(1<<INTF0);
  while (!(GIFR&(1<<INTF0)));
  GIFR|=(1<<INTF0);
  if ((PINB&(1<<PB0))!=0x00) {xxx=PINB; return 1;}
  return 0;
}

void main(void)
{
  mode=0;

  DDRB=0x00;          // ��� ���� �����
  PORTB=0xFE;         // � ���������� � �������

  MCUCR|=0x03;        // ����������
  GIMSK=(1<<INT0);    // �� ��������� ������ �� INT0 (PB1, SCL)

  xxx=PINB;

  while(1)
  {
  waiting_for_start:
    // ������ �� ������?
    if ((PINB&(1<<PB3))==0x00)
    {
      __delay_cycles(CLOCK/1000*300);
      if (mode) mode=0; else mode=1;
    }

    if (mode)
    {
      if (xxx!=PINB)
      {
        if (((PINB&0x03)==0x02)&&(xxx&0x01))
        {
          goto capturing_data;
        }
        xxx=PINB;
      }
    }
    goto waiting_for_start;
  capturing_data:
    // 1
    if (check1()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 1
    if (check1()) goto waiting_for_start;
    // 1
    if (check1()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 1
    if (check1()) goto waiting_for_start;
    // 1
    if (check1()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // 0
    if (check0()) goto waiting_for_start;
    // ��������������� INT0 �� ������ �����
    MCUCR&=~0x03;
    MCUCR|=0x02;
    GIFR|=(1<<INTF0);
    while (!(GIFR&(1<<INTF0)));
    GIFR|=(1<<INTF0);
    MCUCR|=0x03;

    PORTB|=(1<<PB0);DDRB|=(1<<PB0);

    // 0-1
    if (check1()) goto waiting_for_start;
    // 0-1
    if (check1()) goto waiting_for_start;
    // 0-1
    if (check1()) goto waiting_for_start;
    // 1
    if (check1()) goto waiting_for_start;

    // ��������������� INT0 �� ������ �����
    MCUCR&=~0x03;
    MCUCR|=0x02;
    // ���� 2 ������ ������
    GIFR|=(1<<INTF0);
    while (!(GIFR&(1<<INTF0)));
    // ����� SDA
    PORTB&=~(1<<PB0);DDRB|=(1<<PB0);
    // �� ���� ����
    GIFR|=(1<<INTF0);
    while (!(GIFR&(1<<INTF0)));
    GIFR|=(1<<INTF0);
    MCUCR|=0x03;
    // ��������� SDA
    DDRB&=~(1<<PB0);
    // ���� ����� ������ ��������� �������
    goto waiting_for_start;
  }
}
