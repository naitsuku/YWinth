/*
 * Button.cpp
 *
 * Created: 2018/10/25 22:37:22
 *  Author: naitsuku
 */ 

#include "Button.h"

/* ボタンサンプリング値（6ボタン × 8回） */
uint8_t buttonSampleValue[6] = {0};

/* ボタン今回確定値 */
uint8_t buttonDefValue = 0xff;

/* ボタン前回確定値 */
uint8_t buttonDefPrevValue = 0xff;

/* ボタン押下検出(前回未押下⇒今回押下) */
uint8_t buttonPressed = 0;

void buttonInit(){
	/* ポート：入力設定 */
	DDRB &= ~( (1<<BTN1)|(1<<BTN2)|(1<<BTN3)|(1<<BTN4) );
	DDRC &= ~( (1<<BTN5)|(1<<BTN6) );
	
	/* 内部プルアップ有効化 */
	PORTB |= (1<<BTN1)|(1<<BTN2)|(1<<BTN3)|(1<<BTN4);
	PORTC |= (1<<BTN5)|(1<<BTN6);
}

uint8_t buttonGet(){
	uint8_t result=0;
	result  = PINB&((1<<BTN1)|(1<<BTN2)|(1<<BTN3)|(1<<BTN4));
	result |= ( PINC&((1<<BTN5)|(1<<BTN6)) ) >> 2;
	return result;
}

/* ボタン値サンプリング（一回分） */
void buttonSampling(){
	uint8_t braw,i;
	braw = buttonGet(); /* ボタン生値(負論理) */
	
	for(i=0; i<6; i++){ /* ボタン個数分 */
		buttonSampleValue[i] <<= 1; /* サンプリング値をシフト(ｴﾝｷｭｰ) */
		buttonSampleValue[i] |=  ((braw>>i)&0x01); /* 今回サンプリング値を追加 */
	}
}

/* ボタン値平均化処理 */
void buttonAveraging(){
	uint8_t i;
	/* ボタン前回確定値を保存 */
	buttonDefPrevValue = buttonDefValue;
	for(i=0; i<6; i++){ /* ボタン個数分 */
		if( (buttonSampleValue[i]&0x0F) == 0x0F ){ /* 4回一致で値確定 */
			/* ボタン値：Ｈ確定 */
			buttonDefValue |= (1<<i);
		}else if( (buttonSampleValue[i]&0x0F) == 0x00 ){ /* 4回一致で値確定 */
			/* ボタン値：Ｌ確定 */
			buttonDefValue &= ~(1<<i);
		}
	}
}

/* ボタン変化していて，かつ押下中(L)検出 */
void buttonPressDetect(){
	uint8_t result;
	result  = buttonDefValue ^ buttonDefPrevValue; /* ビット変化検出(xor) */
	result  &= (~buttonDefValue); /* 押下中検出 */
	
	buttonPressed |= result; /* 押されたボタンのビットを立てる */
}

/* ボタン値からコマンド取得・複数押された場合は低位ビットから処理 */
uint8_t buttonGetCommand(){
	uint8_t i;
	for(i=0; i<6; i++){
		if( ( (buttonPressed>>i)&0x01 )==0x01 ){
			buttonPressed &= ~(1<<i); /* 押下されてるボタンのビットを下げる */
			return (i+1); /* 押下されてるボタン番号を返す */
		}
	}
	
	return 0; /* 押下なし */
}
