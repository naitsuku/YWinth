/*
 * YWinthCommon.h
 *
 * Created: 2018/10/23 10:23:59
 *  Author: naitsuku
 */ 


#ifndef YWINTHCOMMON_H_
#define YWINTHCOMMON_H_


using namespace std;

/* CPUクロック周波数：16MHz */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define ENABLE  1 /* 有効 */
#define DISABLE 0 /* 無効 */

#define VERSIONCODE "V0.01"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/twi.h>
#include <avr/pgmspace.h> /* 配列ROM配置に必要 */

#include "SPI.h"
#include "Touch.h"
#include "YMF825.h"
#include "Breath.h"
#include "Finger.h"
#include "LCD.h"
#include "Button.h"
#include "Menu.h"
#include "Midi.h"
#include "Tone.h"
#include "Comm.h"

/* 各種プロトタイプ宣言！（クラスに統合予定！） */

/* AVRペリフェラル関連 */
void setIOMode(void);
void ctrlLed(uint8_t state);
void uartInit(void);
void uartPutc(char a);
void uartPuts(char * str);
void initTimer(void);

#endif /* YWINTHCOMMON_H_ */