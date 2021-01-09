/*
 * Button.cpp
 *
 * Created: 2018/10/25 22:37:22
 *  Author: naitsuku
 */ 

#include "Button.h"

/* �{�^���T���v�����O�l�i6�{�^�� �~ 8��j */
uint8_t buttonSampleValue[6] = {0};

/* �{�^������m��l */
uint8_t buttonDefValue = 0xff;

/* �{�^���O��m��l */
uint8_t buttonDefPrevValue = 0xff;

/* �{�^���������o(�O�񖢉����ˍ��񉟉�) */
uint8_t buttonPressed = 0;

void buttonInit(){
	/* �|�[�g�F���͐ݒ� */
	DDRB &= ~( (1<<BTN1)|(1<<BTN2)|(1<<BTN3)|(1<<BTN4) );
	DDRC &= ~( (1<<BTN5)|(1<<BTN6) );
	
	/* �����v���A�b�v�L���� */
	PORTB |= (1<<BTN1)|(1<<BTN2)|(1<<BTN3)|(1<<BTN4);
	PORTC |= (1<<BTN5)|(1<<BTN6);
}

uint8_t buttonGet(){
	uint8_t result=0;
	result  = PINB&((1<<BTN1)|(1<<BTN2)|(1<<BTN3)|(1<<BTN4));
	result |= ( PINC&((1<<BTN5)|(1<<BTN6)) ) >> 2;
	return result;
}

/* �{�^���l�T���v�����O�i��񕪁j */
void buttonSampling(){
	uint8_t braw,i;
	braw = buttonGet(); /* �{�^�����l(���_��) */
	
	for(i=0; i<6; i++){ /* �{�^������ */
		buttonSampleValue[i] <<= 1; /* �T���v�����O�l���V�t�g(�ݷ��) */
		buttonSampleValue[i] |=  ((braw>>i)&0x01); /* ����T���v�����O�l��ǉ� */
	}
}

/* �{�^���l���ω����� */
void buttonAveraging(){
	uint8_t i;
	/* �{�^���O��m��l��ۑ� */
	buttonDefPrevValue = buttonDefValue;
	for(i=0; i<6; i++){ /* �{�^������ */
		if( (buttonSampleValue[i]&0x0F) == 0x0F ){ /* 4���v�Œl�m�� */
			/* �{�^���l�F�g�m�� */
			buttonDefValue |= (1<<i);
		}else if( (buttonSampleValue[i]&0x0F) == 0x00 ){ /* 4���v�Œl�m�� */
			/* �{�^���l�F�k�m�� */
			buttonDefValue &= ~(1<<i);
		}
	}
}

/* �{�^���ω����Ă��āC��������(L)���o */
void buttonPressDetect(){
	uint8_t result;
	result  = buttonDefValue ^ buttonDefPrevValue; /* �r�b�g�ω����o(xor) */
	result  &= (~buttonDefValue); /* ���������o */
	
	buttonPressed |= result; /* �����ꂽ�{�^���̃r�b�g�𗧂Ă� */
}

/* �{�^���l����R�}���h�擾�E���������ꂽ�ꍇ�͒�ʃr�b�g���珈�� */
uint8_t buttonGetCommand(){
	uint8_t i;
	for(i=0; i<6; i++){
		if( ( (buttonPressed>>i)&0x01 )==0x01 ){
			buttonPressed &= ~(1<<i); /* ��������Ă�{�^���̃r�b�g�������� */
			return (i+1); /* ��������Ă�{�^���ԍ���Ԃ� */
		}
	}
	
	return 0; /* �����Ȃ� */
}
