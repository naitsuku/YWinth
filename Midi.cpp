/*
 * Midi.cpp
 *
 * Created: 2018/10/31 23:06:25
 *  Author: naitsuku
 */ 

#include "Midi.h"

/* MIDI初期化 */
void midiInit(){
	/* ＭＩＤＩ有効化 */
	UCSR1B = (1<<TXEN1) | (1<<RXEN1);
	/* ボーレートを31250BPSに設定 */
	UBRR1 = 31;
}

/* MIDIノートオン */
void midiNoteOn(uint8_t ch, uint8_t note, uint8_t vel){
	uint8_t midimsg[4];
	midimsg[0] = MIDI_NOTEON | ch;
	midimsg[0] = 0x90;
	midimsg[1] = note;
	midimsg[2] = vel;
	midimsg[3] = '\0';
	
	/* デバッグ用 */
	//hexdump((uint8_t*)midimsg, 3);
	
	midiPuts(midimsg);
}

/* MIDIノートオフ */
void midiNoteOff(uint8_t ch, uint8_t note){
	uint8_t midimsg[4];
	midimsg[0] = MIDI_NOTEOF | ch;
	midimsg[0] = 0x90;
	midimsg[1] = note;
	midimsg[2] = 0x00; /* ベロシティ=0 ⇒ noteoff */
	midimsg[3] = '\0';
	
	/* デバッグ用 */
	//hexdump((uint8_t*)midimsg, 3);
	
	midiPuts(midimsg);
}

/* MIDIオールノートオフ */
void midiAllNoteOff(uint8_t ch){
	uint8_t midimsg[4];
	midimsg[0] = 0xB0 | ch;
	midimsg[1] = 0x7B;
	midimsg[2] = 0x00;
	midimsg[3] = '\0';
	
	midiPuts(midimsg);
}

/* MIDIアフタータッチ */
void midiAfterTouch(uint8_t ch, uint8_t vel){
	uint8_t midimsg[3];
	
	midimsg[0] = MIDI_CHPRSH | ch;
	midimsg[1] = vel;
	midimsg[2] = '\0';
	
	midiPuts(midimsg);
}

/* MIDIコントロールチェンジ */
void midiControlChange(){
	
}

/* MIDIプログラムチェンジ */
void midiProgramChange(){
	
}

void midiPutc(uint8_t a){
	/* MIDIに一文字出力 */
	while(!(UCSR1A&0b00100000));
	UDR1 = a;
}

void midiPuts(uint8_t * str){
	uint8_t i;
	/*
	while(*str != '\0'){
		midiPutc(*str);
		str++;
	} */
	
	for(i=0; i<3; i++){
		midiPutc(str[i]);
	}
}