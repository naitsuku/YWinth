/*
 * Midi.cpp
 *
 * Created: 2018/10/31 23:06:25
 *  Author: naitsuku
 */ 

#include "Midi.h"

/* MIDI������ */
void midiInit(){
	/* �l�h�c�h�L���� */
	UCSR1B = (1<<TXEN1) | (1<<RXEN1);
	/* �{�[���[�g��31250BPS�ɐݒ� */
	UBRR1 = 31;
}

/* MIDI�m�[�g�I�� */
void midiNoteOn(uint8_t ch, uint8_t note, uint8_t vel){
	uint8_t midimsg[4];
	midimsg[0] = MIDI_NOTEON | ch;
	midimsg[0] = 0x90;
	midimsg[1] = note;
	midimsg[2] = vel;
	midimsg[3] = '\0';
	
	/* �f�o�b�O�p */
	//hexdump((uint8_t*)midimsg, 3);
	
	midiPuts(midimsg);
}

/* MIDI�m�[�g�I�t */
void midiNoteOff(uint8_t ch, uint8_t note){
	uint8_t midimsg[4];
	midimsg[0] = MIDI_NOTEOF | ch;
	midimsg[0] = 0x90;
	midimsg[1] = note;
	midimsg[2] = 0x00; /* �x���V�e�B=0 �� noteoff */
	midimsg[3] = '\0';
	
	/* �f�o�b�O�p */
	//hexdump((uint8_t*)midimsg, 3);
	
	midiPuts(midimsg);
}

/* MIDI�I�[���m�[�g�I�t */
void midiAllNoteOff(uint8_t ch){
	uint8_t midimsg[4];
	midimsg[0] = 0xB0 | ch;
	midimsg[1] = 0x7B;
	midimsg[2] = 0x00;
	midimsg[3] = '\0';
	
	midiPuts(midimsg);
}

/* MIDI�A�t�^�[�^�b�` */
void midiAfterTouch(uint8_t ch, uint8_t vel){
	uint8_t midimsg[3];
	
	midimsg[0] = MIDI_CHPRSH | ch;
	midimsg[1] = vel;
	midimsg[2] = '\0';
	
	midiPuts(midimsg);
}

/* MIDI�R���g���[���`�F���W */
void midiControlChange(){
	
}

/* MIDI�v���O�����`�F���W */
void midiProgramChange(){
	
}

void midiPutc(uint8_t a){
	/* MIDI�Ɉꕶ���o�� */
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