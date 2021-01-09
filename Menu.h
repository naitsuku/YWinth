/*
 * Menu.h
 *
 * Created: 2018/10/29 9:12:10
 *  Author: naitsuku
 */ 


#ifndef MENU_H_
#define MENU_H_

#include "YWinthCommon.h"


/* ���j���[�����` */
#define UP		 1
#define DN		-1
#define RST      0
#define NEXT	 1
#define PREV	-1
#define NOCNG	 0

/* ���j���[���ڔԍ��ő�l(���j���[���ڐ�-1) */
#define MENUITEM_MAX_NO 5
#define CHR_SIZE 20

/* �{�^���N���b�N���C�x���g�|�C���^ */
typedef void (*EVENT)();

/* �� ���j���[���ږ��e�[�u�� */
const char MENUITEM_STR_TBL[MENUITEM_MAX_NO+1][CHR_SIZE] PROGMEM = {
	"MasterVolume ",
	"PrisetTone   ",
	"Transpose    ",
	"Octave       ",
	"BreathLevel  ",
	"Finger       "
};

const char* const MENUITEMS_TBL[] PROGMEM = {
	MENUITEM_STR_TBL[0],
	MENUITEM_STR_TBL[1],
	MENUITEM_STR_TBL[2],
	MENUITEM_STR_TBL[3],
	MENUITEM_STR_TBL[4],
	MENUITEM_STR_TBL[5]
};

/* ���ݒ�l���j�[�N���e�[�u�� */

/* ���F��ð��� */
const char TONENAME_STR_TBL[][8] PROGMEM = {
	"SinWave",
	"Brass 1",
	"Brass 2",
	"SquWave",
	"SawWave",
	"E.Piano",
	"Tnr.Sax"
};

/* �ڒ�(�g�����X�|�[�Y)�e�[�u�� */
const char TRANSPOSE_STR_TBL[12][8] PROGMEM = {
	" G   -5",
	" Ab  -4",
	" A   -3",
	"[Bb] -2",
	" B   -1",
	"[C ]  0",
	" Db  +1",
	" D   +2",
	"[Eb] +3",
	" E   +4",
	"[F ] +5",
	" F#  +6"
};

/* �^�w�e�[�u�� */
const char FINGERPTN_STR_TBL[4][8] PROGMEM = {
	" YWinth",
	"  Flute",
	"    Sax",
	"DigHorn"
};

/* �ݒ�l���j�[�N���|�C���^�e�[�u�� */
const char* const MENUITEM_VALUENAME_TBL[] PROGMEM = {
	/* PrisetTone */
	TONENAME_STR_TBL[0], /* 0x00 */
	TONENAME_STR_TBL[1],
	TONENAME_STR_TBL[2],
	TONENAME_STR_TBL[3],
	TONENAME_STR_TBL[4],
	TONENAME_STR_TBL[5],
	TONENAME_STR_TBL[6],
	
	/* Transpose */
	TRANSPOSE_STR_TBL[0], /* 0x07 */
	TRANSPOSE_STR_TBL[1],
	TRANSPOSE_STR_TBL[2],
	TRANSPOSE_STR_TBL[3],
	TRANSPOSE_STR_TBL[4],
	TRANSPOSE_STR_TBL[5],
	TRANSPOSE_STR_TBL[6],
	TRANSPOSE_STR_TBL[7],
	TRANSPOSE_STR_TBL[8],
	TRANSPOSE_STR_TBL[9],
	TRANSPOSE_STR_TBL[10],
	TRANSPOSE_STR_TBL[11],
	
	FINGERPTN_STR_TBL[0], /* 0x18 */
	FINGERPTN_STR_TBL[1],
	FINGERPTN_STR_TBL[2],
	FINGERPTN_STR_TBL[3]
};

/* �ݒ�l���j�[�N���ۑ��ʒu�A�h���X�I�t�Z�b�g�e�[�u�� */
const uint8_t MENUITEM_VALUENAME_ADDR_TBL[] PROGMEM = {
	255, /* MasterVolume */
	0,   /* PrisetTone */
	7,   /* Transpose */
	255,
	255,
	18
};


/* �� ���j���[�ψʃe�[�u�� */
/* �߂��Fchar���������邩�͕s��Ȃ̂ŕ����������Ƃ��́hsigned char�h�Ƃ��邱�ƁI */

/* �ݒ�\�ŏ��l */
const signed char MENUITEM_MIN_TBL[] PROGMEM = {
	0,		/* MasterVolume */
	0,		/* PrisetTone */
	-5,	/* Transpose */
	-4,		/* Octave */
	0,		/* BreathLevel */
	0		/* FingerPattern */
};

/* �ݒ�\�ő�l */
const signed char MENUITEM_MAX_TBL[] PROGMEM = {
	31,		/* MasrerVolume */
	6,		/* PrisetTone */
	6,		/* Transpose */
	4,		/* Octave */
	100,	/* BreathLevel*/
	3		/* FingerPattern */
};

/* �����l�e�[�u�� */
const signed char MENUITEM_DEFAULT_TBL[] PROGMEM = {
	20,		/* MasrerVolume */
	0,		/* PrisetTone */
	0,		/* Transpose */
	0,		/* Octave */
	50,		/* BreathLevel*/
	0		/* FingerPattern */
};

/* ���ݒ�l���j�[�N���ێ��̗L���e�[�u��
	 �ݒ�l�����l:DISABLE �ݒ�l�����O������:ENABLE */
const signed char MENUITEM_HAS_VALUENAME[] PROGMEM = {
	DISABLE,		/* MasrerVolume */
	ENABLE,			/* PrisetTone */
	ENABLE,		/* Transpose */
	DISABLE,		/* Octave */
	DISABLE,		/* BreathLevel*/
	ENABLE			/* FingerPattern */
};


/* �� ���j���[�C�x���g */
void ev_setOctave();
void ev_setTranspose();
void ev_setPrisetTone();

/* ���j���[�C�x���g�e�[�u���F�l�ҏW���̓��� */
#if 1
const EVENT MENUITEM_EVENT_TBL[] = { /* memo... �֐��|�C���^��PROGMEM�ɂ����Ⴂ���Ȃ��̂����H */
	NULL,				/* MasrerVolume */
	ev_setPrisetTone,	/* PrisetTone */
	ev_setTranspose,	/* Transpose */
	ev_setOctave,		/* Octave */
	NULL,				/* BreathLevel*/
	NULL				/* FingerPattern */
};
#endif

/* ���j���[�C�x���g�e�[�u���FOK�N���b�N������ */
#if 0
static (*MENUITEM_EVENT_TBL[])() PROGMEM{
	changeMasterVolume,		/* MasrerVolume */
	setPrisetTone,			/* PrisetTone */
	setTranspose,			/* Transpose */
	setOctave,				/* Octave */
	setBreathLevel,			/* BreathLevel*/
	setFingerPattern		/* FingerPattern */
}
#endif

/* ���j���[�C�x���g�e�[�u���F��ݾكN���b�N������ */
#if 0
static (*MENUITEM_EVENT_TBL[])() PROGMEM{
	changeMasterVolume,		/* MasrerVolume */
	setPrisetTone,			/* PrisetTone */
	setTranspose,			/* Transpose */
	setOctave,				/* Octave */
	setBreathLevel,			/* BreathLevel*/
	setFingerPattern		/* FingerPattern */
}
#endif

/* ���ݑI�𒆃��j���[���� */
extern uint8_t menuSelectingItem;

/* ���j���[�ݒ�l */
extern signed char menuItemSetValues[MENUITEM_MAX_NO+1];

/* �I�𒆍��ڂ̐ݒ�l */
extern signed char * menuItemValue;

/* ���j���[�\���X�V */
void menuRefreshDisplay();

/* ���j���[���ڈړ� */
void menuMoveItem(signed char arg);

/* ���j���[�ݒ�l���� */
void menuAdjValue(signed char arg);

/* ���j���[���̃A�N�e�B�r�e�B */
void menuActivity();

/* ���j���[���������� */
void menuInit();


#endif /* MENU_H_ */