/*
 * Comm.h
 *
 * Created: 2018/12/6 9:22:48
 *  Author: naitsuku
 */ 


#ifndef COMM_H_
#define COMM_H_

#include "YWinthCommon.h"

/* �ʐM�֘A */

/* UART�{�[���[�g�F115200bps */
#define BAUD 57600UL

/* ��M�o�b�t�@ */
extern volatile char RxBuff[256];
extern volatile uint8_t RxBuffWrPt; /* �o�b�t�@�������񂾈ʒu */
extern volatile uint8_t RxBuffRdPt; /* �o�b�t�@���ɓǂݍ��ވʒu */

extern char RcvStrBuff[256];

extern volatile uint8_t CmdEn;

/* ��M���荞�݁˂Ƃ肠�����o�b�t�@�ɓ���Ƃ��˃��C�����[�v����o�b�t�@�ɂȂ񂩂���Γǂ� */

void uartInit(void);
void uartPutc(char a);
void uartPuts(char * str);

/* UART�ꕶ���ǂݍ��� */
char uartGetc();
/* �R�}���h������݂̂����o�� */
void uartGetCmdStr();

void cmdParse();

void hexdump(uint8_t * hex, int len);

#endif /* COMM_H_ */