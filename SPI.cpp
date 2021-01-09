/*
 * SPI.cpp
 *
 * Created: 2018/10/23 13:16:12
 *  Author: naitsuku
 */ 


using namespace std;
#include "SPI.h"

void spiInit(){
	/* SPI�ݒ� */
	SPCR |= 0b01010001;
	/*	  	  |||||||+- SPR0 SPI�N���b�N�I��
			  ||||||+-- SPR1 
			  |||||+--- CPHA
			  ||||+---- CPOL
			  |||+----- MSTR ��/�����u�ݒ�(MSR=1/SLV=0)
			  ||+------ DORD
			  |+------- SPE  SPI����
			  +-------- SPIE SPI��������
	*/
	
	/* SPI�g�p�|�[�g�ݒ� */
	DDRB  |= 0b10110000;
	/* MISO:���� MOSI,SCK,SS:�o�� */
}
void spiSend(uint8_t data){
	SPDR = data; /* SPI�f�[�^���W�X�^�ɏ�������ő��M�J�n */
	while((SPSR & (1<<SPIF)) == 0); /* ���M�����҂� */
}

uint8_t spiRead(){
	uint8_t data;
	SPDR = 0; /* �_�~�[�f�[�^(�N���b�N�̂ݑ��o) */
	while((SPSR & (1<<SPIF)) == 0); /* ���M�����҂� */
	data = SPDR; /* ��M�f�[�^�Ԃ� */
	return data;
}

void spiCtrlCs(uint8_t en){
	
	if(en == DISABLE){
		/* SPI CS=H ������ */
		PORTB|= 0b00010000;
		/*	   	  |||||||+- SCK  SPI�N���b�N  �o��
				  ||||||+-- MISO SPIϽ���     ����
				  |||||+--- MOSI SPIϽ����    �o��
				  ||||+---- SS   SPI�ڰ�޾ڸ� �o��
		*/
	}else{
		/* SPI CS=L �L���� */
		PORTB&= 0b11101111;
		/*	   	  |||||||+- SCK  SPI�N���b�N  �o��
				  ||||||+-- MISO SPIϽ���     ����
				  |||||+--- MOSI SPIϽ����    �o��
				  ||||+---- SS   SPI�ڰ�޾ڸ� �o��
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