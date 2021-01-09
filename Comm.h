/*
 * Comm.h
 *
 * Created: 2018/12/6 9:22:48
 *  Author: naitsuku
 */ 


#ifndef COMM_H_
#define COMM_H_

#include "YWinthCommon.h"

/* 通信関連 */

/* UARTボーレート：115200bps */
#define BAUD 57600UL

/* 受信バッファ */
extern volatile char RxBuff[256];
extern volatile uint8_t RxBuffWrPt; /* バッファ書き込んだ位置 */
extern volatile uint8_t RxBuffRdPt; /* バッファ次に読み込む位置 */

extern char RcvStrBuff[256];

extern volatile uint8_t CmdEn;

/* 受信割り込み⇒とりあえずバッファに入れとく⇒メインループからバッファになんかあれば読む */

void uartInit(void);
void uartPutc(char a);
void uartPuts(char * str);

/* UART一文字読み込み */
char uartGetc();
/* コマンド文字列のみを取り出し */
void uartGetCmdStr();

void cmdParse();

void hexdump(uint8_t * hex, int len);

#endif /* COMM_H_ */