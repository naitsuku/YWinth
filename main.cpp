/*
 * �E�B���h�V���Z�T�C�U�[����v���W�F�N�g�wYwinth�x
 * YWinth_MainMPU.cpp
 *
 * Created: 2018/10/02 10:39:11
 * Author : naitsuku
 */ 

using namespace std;

/* ���ʃ��C�u���� */
#include "YWinthCommon.h"

/* �O���[�o���ϐ� */
volatile uint8_t ledstate=0;
volatile uint8_t cnt=0;
volatile bool executePermission = ENABLE;


/* �^�C�}�����ݔ���������
   �� 1000Hz T=1mS */
ISR (TIMER0_COMPA_vect){
	/* 120Hz 8mS�Ɉ�x���C���������s */
	if(cnt == 3){
		/* ���C���������s�����s */
		executePermission = ENABLE; /* ���s���̊J���̓��C�������ōs�� */
		cnt = 0;
	}else{
		cnt++;
	}
	/* �`���^�����O�������� 4mS��v�Ń{�^���l�m�� */
	buttonSampling();		/* 1.�T���v�����O */
	buttonAveraging();		/* 2.���ω� */
	buttonPressDetect();	/* 3.�������m */
}

/* USART0��M���荞�� */
ISR (USART0_RX_vect){
	RxBuff[RxBuffWrPt] = UDR0; /* �o�b�t�@�ɐς� */
	RxBuffWrPt++;
}

/* ���C������ */
int main(void)
{
	/* ���C�������F���������� */
	char str[100];
	uint32_t data;
	long bdata;
	uint8_t keyval;
	uint16_t noteNum, noteNum_old=0;
	char transpose=12;
	uint8_t vel, vel_old=0;
	
	spiInit();
	spiCtrlCs(DISABLE);
	
	setIOMode();
	buttonInit();
	//uartInit();
	
	soundInit();
	setTone();
	setCh();
	
	keyOnNoteNoWithVovol(60, 31);
	
	uartInit();
	
	/* UART�X�^�[�g�A�b�vү���ޑ��M */
	sprintf(str, "***      YWinth Serial Console      ***\nFirmware version: %s\n", VERSIONCODE);
	uartPuts(str);
	uartPuts("4 Operator FM Sound Wind Synthesizer.\n");
	uartPuts("Designed by K.Yazawa.\n");
	uartPuts("This is Serial Console.\n");
	uartPuts("Please input command.\n");

	/* MIDI���������� */
	midiInit();

	/* LCD�\������ LCD�͒x���̂ŋC������ */
	_delay_ms(100);
	lcdInit();
	_delay_ms(100);
	lcdPrint("YWinth");
	_delay_ms(20);
	lcdSetCursor(11,1);
	_delay_ms(20);
	lcdPrint(VERSIONCODE);
	_delay_ms(20);
	
	keyOff();
	
	initTimer();
	breathInit();
	
	cnt = 0;
	
	_delay_ms(500); /* ������҂� */
	setBreathOffset();
	_delay_ms(500);
	midiAllNoteOff(0x01);
	
	menuInit();
	/* �����������C���荞�ݗL�����I */
	sei();
	
	/* ���C�������F���[�v���� */
	
	while(1){
		
		/* �^�C�}�ɂ�8mS�����Ŏ��s�����s */
		/* ���s���m�F�����s�\�Ȃ���s */
		while(executePermission == DISABLE);
		
		/* ������ ���t���� ������ */
			
		/* �u���X�f�[�^�擾 �� �x���V�e�B���� */
		bdata = getBreathOffsetValue();
		data = breathToVovol(bdata);
		vel = breathToVelocity(bdata);
	
		
		/* �^�b�`�Z���T�f�[�^�擾 �� �m�[�g�i���o�[���� */
		keyval = touchGet();
		noteNum = fingerToNoteNum(keyval);
			
		/* �m�[�g�i���o�[�{�x���V�e�B �� �L�[�I�� */
		if(noteNum != 0){
			keyOnNoteNoWithVovol(noteNum, data);
		}else{
			keyOff();
		}
		
		uartGetCmdStr();
		sprintf(str, "%d\n", data);
		//uartPuts(str);
		
		/* MIDI���M���� */
#if 1   /* �Ȃ񂩂��܂������Ȃ�... �n�[�h�E�F�A�̃f�o�b�O���Ȃ��Ƃ����Ȃ��C������ */
		/*���I���˃m�[�g�I�t */
		if( (vel<20) ){ 
			if(vel_old>20){
				midiNoteOn(0x01, noteNum_old, 0);
				keyOff();
			}
		}else
		if( ((vel_old<20)&&(vel>=20)) ){ /* ���J�n�Ńm�[�g�I�� */
			midiNoteOn(0x01, noteNum, vel);
			//keyOnNoteNoWithVovol(noteNum, data);
		}else{
			//midiAfterTouch(0x01, vel);
			//keyOnNoteNoWithVovol(noteNum, data);
		}
		
		//_delay_ms(30);
		
		
		/* �߂��F�m�[�g�I�����o�̏����͊֐��Ȃǂ����Ă��������܂Ƃ߂���
				�u���X�Ƀq�X�e���V�X������Ƃ悢�C������ */
#endif

/*
		midiNoteOn(0x00,60, 0x50);
		_delay_ms(200);
		//midiNoteOff(0x01,60);
		midiNoteOn(0x00,60, 0x00);
		_delay_ms(200);
*/
		vel_old = vel;
		noteNum_old = noteNum;
		
		/* ���j���[���� */
		menuActivity();
			
		/* ���s����� */
		executePermission = DISABLE;
		
	}
}

// PIN13(ARD) = PB5
void setIOMode(void){
	DDRB |= 0b00100000;
}

void ctrlLed(uint8_t state){
	uint8_t reg;
	reg = PORTB;
	reg &= 0b11001111;
	reg |= state << 5;
	PORTB = reg;
}


void initTimer(void)
{
	/* Timer0: 8bit�^�C�}�ݒ� */
	TCCR0A = 0b00000010; /* �^�C�}���䃌�W�X�^�` */
	
	/* CTC����I */
	TCCR0B = 0b00000101; /* �^�C�}���䃌�W�X�^�a */
	/*              ||+- CS00 �ۯ�����I��
                    |+-- CS01 101 : 1024���� 
					+--- CS02 16M/1024 = 15.625KHz
					�^�C�}�o�� 61.04Hz T=16.384mS
	*/
	
	OCR0A = 16; /* �R���y�A�Ώ�(255�̏ꍇ �W������Ɠ���) */
	/* ��1mS���� */
	
	TIMSK0 = 0b00000010; /* �^�C�}�����}�X�N���W�X�^ */
	/*               +- OCIE0A ��� ����߯ĺ��߱ϯ�A�����L��
	*/
	
}



