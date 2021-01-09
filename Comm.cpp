/*
 * Comm.cpp
 *
 * Created: 2018/12/6 9:22:35
 *  Author: naitsuku
 */ 

#include "Comm.h"

/* 受信バッファ */
volatile char RxBuff[256]; /* 256バイトリングバッファ */
char RcvStrBuff[256];

volatile uint8_t RxBuffWrPt=0; /* バッファ書き込んだ位置 */
volatile uint8_t RxBuffRdPt=0; /* バッファ次に読み込む位置 */
/* ↑オーバーフローさせてリングバッファとしてつかう */

volatile uint8_t CmdEn;

/* コマンド形式 … @ではじまり，セミコロンでおわる！
   『@fl FF,FF,FF,32,4F,2D,D5;』 */

void uartInit(void){
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);
	UBRR0H = (F_CPU/16/BAUD)>>8; // -1なければ115200いける（意味不明）http://mibc.blog.fc2.com/blog-entry-17.html
	UBRR0L = (F_CPU/16/BAUD);
}

void uartPutc(char a){
	while(!(UCSR0A&0b00100000));
	UDR0 = a;
}

void uartPuts(char * str){
	while(*str != '\0'){
		uartPutc(*str);
		str++;
	}
}

char uartGetc(){
	char data;
	char str[20];
	if(RxBuffRdPt != RxBuffWrPt){ /* Rd位置とWr位置異なる場合データあり */
		
		/* 受信デバッグ用 */
		//sprintf(str,"r:%d w:%d buf:%c\n", RxBuffRdPt, RxBuffWrPt, RxBuff[RxBuffRdPt]);
		//uartPuts(str);
		
		data = RxBuff[RxBuffRdPt];
		RxBuffRdPt++;
		
		/* エコーバック */
		uartPutc(data);
		
		return data;
	}else{
		return (char)0xFF; /* データなし */	
	}
}

void uartGetCmdStr(){
	static uint8_t i=0;
	char chr;
	while(RxBuffRdPt != RxBuffWrPt){
		
		chr = uartGetc();
		if(chr == '@'){
			i = 0;
		}else
		if(chr == ';'){
			RcvStrBuff[i] = '\0';
			uartPuts(RcvStrBuff);
			uartPuts("\n");
			cmdParse();
			break;
		}
		RcvStrBuff[i] = chr;
		i++;
	}
	RcvStrBuff[i] = '\0';
	
	
}

void cmdParse(){
	char opcode[3]; /* オペレーションｺｰﾄﾞ(2文字) */
	char *arg;
	
	opcode[0] = RcvStrBuff[1];
	opcode[1] = RcvStrBuff[2];
	opcode[2] = '\0';
	
	arg = &RcvStrBuff[4];
	
	if(strcmp(opcode, "rm") == 0){
		uartPuts("remote\n");
		switch(arg[0]){
			case 'U':
				menuAdjValue(UP);
				break;
			case 'D':
				menuAdjValue(DN);
				break;
			case 'N':
				menuMoveItem(NEXT);
				break;
			case 'P':
				menuMoveItem(PREV);
				break;
		}
		menuRefreshDisplay();
	}else
	if(strcmp(opcode, "fl") == 0){
		//toneRead(&Tone);
	}
	
}

void hexdump(uint8_t * hex, int len){
	char str[8];
	int i;
	for(i=0; i<len; i++){
		sprintf(str, "%x ", hex[i]);
		uartPuts(str);
	}
	uartPuts("\n");
}