/*
 * LCD.h
 *
 * Created: 2018/10/25 21:30:35
 *  Author: naitsuku
 */ 


#ifndef LCD_H_
#define LCD_H_

#define LCD_ADDR 0x3E
#define LCD_WAIT 300 /* us */
/* ?���߂ɑ҂��Ȃ��ƕ\������Ȃ��I */
#define LCD_LONG_WAIT 200 /* ms */

#include "YWinthCommon.h"

/* LCD���������� */
void lcdInit();

/* LCD�R�}���h���M */
void lcdSendCmd(uint8_t cmd);

/* LCD�������M */
void lcdSendData(uint8_t data);

/* LCD�̔C�ӂ̈ʒu�Ɉꕶ���\�� */
void lcdPutc(char ch, uint8_t posx, uint8_t posy);

void lcdPrint(char * str);

void lcdSetCursor(int x, int y);

#endif /* LCD_H_ */