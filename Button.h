/*
 * Button.h
 *
 * Created: 2018/10/25 22:37:02
 *  Author: naitsuku
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#include "YWinthCommon.h"

/* BTN1~4 : PORTB */
#define BTN1 0
#define BTN2 1
#define BTN3 2
#define BTN4 3

/* BTN5~6: PORTC */
#define BTN5 6
#define BTN6 7

/* �{�^������` */
#define BTN_UP 1 /* �ް����؁{ */
#define BTN_DN 4 /* �ް�����-  */
#define BTN_PR 2 /* �O���� */
#define BTN_NX 3 /* ������ */
#define BTN_OK 5 /* OK */
#define BTN_CA 6 /* �L�����Z�� */

/* �{�^���T���v�����O�l�i6�{�^�� �~ 8��j */
extern uint8_t buttonSampleValue[6];

/* �{�^������m��l */
extern uint8_t buttonDefValue;

/* �{�^���O��m��l */
extern uint8_t buttonDefPrevValue;

/* �{�^���������o(�O�񖢉����ˍ��񉟉�) */
extern uint8_t buttonPressed;


/* ����{�^���|�[�g������ */
void buttonInit();

/* �{�^�����̒l�擾 */
uint8_t buttonGet();

/* �{�^���l�T���v�����O */
void buttonSampling();

/* �{�^���l���T���v�����O�l���畽�ω�(��v���o) */
void buttonAveraging();

/* �{�^���������o �V���ɉ����ꂽ�{�^���̃r�b�g�𗧂Ă� */
void buttonPressDetect();


uint8_t buttonGetCommand();


#endif /* BUTTON_H_ */