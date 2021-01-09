/*
 * YWinthCommon.h
 *
 * Created: 2018/10/23 10:23:59
 *  Author: naitsuku
 */ 


#ifndef YWINTHCOMMON_H_
#define YWINTHCOMMON_H_


using namespace std;

/* CPU�N���b�N���g���F16MHz */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define ENABLE  1 /* �L�� */
#define DISABLE 0 /* ���� */

#define VERSIONCODE "V0.01"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/twi.h>
#include <avr/pgmspace.h> /* �z��ROM�z�u�ɕK�v */

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

/* �e��v���g�^�C�v�錾�I�i�N���X�ɓ����\��I�j */

/* AVR�y���t�F�����֘A */
void setIOMode(void);
void ctrlLed(uint8_t state);
void uartInit(void);
void uartPutc(char a);
void uartPuts(char * str);
void initTimer(void);

#endif /* YWINTHCOMMON_H_ */