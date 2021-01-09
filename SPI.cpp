/*
 * SPI.cpp
 *
 * Created: 2018/10/23 13:16:12
 *  Author: naitsuku
 */ 


using namespace std;
#include "SPI.h"

void spiInit(){
	/* SPI設定 */
	SPCR |= 0b01010001;
	/*	  	  |||||||+- SPR0 SPIクロック選択
			  ||||||+-- SPR1 
			  |||||+--- CPHA
			  ||||+---- CPOL
			  |||+----- MSTR 主/副装置設定(MSR=1/SLV=0)
			  ||+------ DORD
			  |+------- SPE  SPI許可
			  +-------- SPIE SPI割込許可
	*/
	
	/* SPI使用ポート設定 */
	DDRB  |= 0b10110000;
	/* MISO:入力 MOSI,SCK,SS:出力 */
}
void spiSend(uint8_t data){
	SPDR = data; /* SPIデータレジスタに書き込んで送信開始 */
	while((SPSR & (1<<SPIF)) == 0); /* 送信完了待ち */
}

uint8_t spiRead(){
	uint8_t data;
	SPDR = 0; /* ダミーデータ(クロックのみ送出) */
	while((SPSR & (1<<SPIF)) == 0); /* 送信完了待ち */
	data = SPDR; /* 受信データ返す */
	return data;
}

void spiCtrlCs(uint8_t en){
	
	if(en == DISABLE){
		/* SPI CS=H 無効化 */
		PORTB|= 0b00010000;
		/*	   	  |||||||+- SCK  SPIクロック  出力
				  ||||||+-- MISO SPIﾏｽﾀｲﾝ     入力
				  |||||+--- MOSI SPIﾏｽﾀｱｳﾄ    出力
				  ||||+---- SS   SPIｽﾚｰﾌﾞｾﾚｸﾄ 出力
		*/
	}else{
		/* SPI CS=L 有効化 */
		PORTB&= 0b11101111;
		/*	   	  |||||||+- SCK  SPIクロック  出力
				  ||||||+-- MISO SPIﾏｽﾀｲﾝ     入力
				  |||||+--- MOSI SPIﾏｽﾀｱｳﾄ    出力
				  ||||+---- SS   SPIｽﾚｰﾌﾞｾﾚｸﾄ 出力
		*/
	}
}

void spiRegWrite(uint8_t addr, uint8_t data){
	spiCtrlCs(ENABLE);
	spiSend(addr);
	spiSend(data);
	spiCtrlCs(DISABLE);
}


void spiRegBarstWrite(uint8_t addr, uint8_t * data, uint8_t dsize){
	spiCtrlCs(ENABLE);
	spiSend(addr);
	for(int i=0; i<dsize; i++){
		spiSend(data[i]);
	}
	spiCtrlCs(DISABLE);
}

uint8_t spiRegRead(uint8_t addr){
	uint8_t data;
	spiCtrlCs(ENABLE);
	spiSend( 0b10000000 | addr ); /* RW=1(read) set */
	data = spiRead();
	spiCtrlCs(DISABLE);
	return data;
}