/*
 * Tone.cpp
 *
 * Created: 2018/12/1 22:15:35
 *  Author: naitsuku
 */ 

#include "Tone.h"

tonedata_t Tone;

void setPrisetTone(uint8_t tone_num){
	uint8_t tone_data[35];
	int i;
	
	/* テーブルからトーンデータ取得 */
	for(i=0; i<35; i++){
		tone_data[i] = pgm_read_word(&TONE_TBL[tone_num][i]);
	}
	
	/* トーンデータ書き込み */
	spiRegWrite( 0x08, 0xF6 );
	_delay_ms(1);
	spiRegWrite( 0x08, 0x00 );
	
	spiRegBarstWrite( 0x07, &tone_data[0], 35 );
	
	toneRead(tone_data, &Tone);
	toneDump(&Tone);
}

/* トーンデータレジスタ読み込み ⇒ 読み込みはしない方向で！！！ */
void toneRegRead(uint8_t * tone_data){
	int i;
	for(i=0; i<35; i++){
		tone_data[i] = spiRegRead(0x30+i);
	}
}

/* トーンデータレジスタ書き込み */
void toneRegWrite(uint8_t * tone_data){
	/* トーンデータ書き込み */
	spiRegWrite( 0x08, 0xF6 );
	_delay_ms(1);
	spiRegWrite( 0x08, 0x00 );
	
	spiRegBarstWrite( 0x07, &tone_data[0], 35 );
}

void toneWrite(tonedata_t * tone){
	uint8_t tone_data[35];
	toneRegRead(tone_data);
	uint8_t i,adoft;
	
	/* 構造体からレジスタに押し込む */
	tone_data[0] = 0x81; /* header */
	tone_data[1] = (tone->bo << 3);
	tone_data[2] = (tone->lfo << 6)|(tone->arg);
	
	for(i=0; i<4; i++){
		adoft = 7*i;
		tone_data[adoft+3] = tone->op[i].sr<<4;
		tone_data[adoft+4] = (tone->op[i].rr<<4) | (tone->op[i].dr);
		tone_data[adoft+5] = (tone->op[i].ar<<4) | (tone->op[i].sl); 
		tone_data[adoft+6] = tone->op[i].tl<<2;
	}
	
}

void toneRead(uint8_t * tone_data, tonedata_t * tone){
	uint8_t i;
	uint8_t adoft;
	
	tone->bo  = tone_data[1]&0x3;
	tone->lfo = (tone_data[2]&0xC0)>>6;
	tone->arg = tone_data[2]&0x7; 
	
	for(i=0; i<4; i++){
		adoft = 7*i;
		tone->op[i].sr = (tone_data[adoft+3]&0xF0)>>4;
		tone->op[i].rr = (tone_data[adoft+4]&0xF0)>>4;
		tone->op[i].ar = (tone_data[adoft+5]&0xF0)>>4;
		tone->op[i].tl = (tone_data[adoft+6]&0xFC)>>2;
		tone->op[i].dr = (tone_data[adoft+4]&0x0F);
		tone->op[i].sl = (tone_data[adoft+5]&0x0F);
	
		tone->op[i].xof = (tone_data[adoft+3]&0x08)>>3;
		tone->op[i].ksr = (tone_data[adoft+3]&0x01);
		tone->op[i].ksl = (tone_data[adoft+6]&0x03);
		tone->op[i].dam = (tone_data[adoft+7]&0x60)>>5;
		tone->op[i].eam = (tone_data[adoft+7]&0x10)>>4;
		tone->op[i].dvb = (tone_data[adoft+7]&0x06)>>1;
		tone->op[i].evb = (tone_data[adoft+7]&0x01);
	
		tone->op[i].mul = (tone_data[adoft+8]&0xF0)>>4;
		tone->op[i].det = (tone_data[adoft+8]&0x07);
		tone->op[i].ws  = (tone_data[adoft+9]&0xF8)>>3;
		tone->op[i].fb  = (tone_data[adoft+9]&0x03);
	}
	
	/* レジスタデータから構造体に読み出し */
}

void toneDump(tonedata_t * tone){
	uint8_t i;
	char str[64];
	
	sprintf(str, "bo:%x lfo:%x arg:%x\n", tone->bo, tone->lfo, tone->arg);
	uartPuts(str);
	
	for(i=0; i<4; i++){
		
		sprintf(str, "-OP%02d-\n", i);
		uartPuts(str);
		
		sprintf(str, "sr:%x rr:%x ar:%x tl:%x dr:%x sl:%x\n",
				tone->op[i].sr, tone->op[i].rr, tone->op[i].ar,
				tone->op[i].tl, tone->op[i].dr, tone->op[i].sl);
		uartPuts(str);
		
		sprintf(str, "xof:%x ksr:%x ksl:%x dam:%x eam:%x dvb:%x evb:%x\n",
		tone->op[i].xof, tone->op[i].ksr, tone->op[i].ksl,
		tone->op[i].dam, tone->op[i].eam, tone->op[i].dvb, tone->op[i].evb);
		uartPuts(str);
		
		sprintf(str, "mul:%x det:%x ws:%x fb:%x\n",
				tone->op[i].mul, tone->op[i].det,
				tone->op[i].ws, tone->op[i].fb);
		uartPuts(str);
	}
}