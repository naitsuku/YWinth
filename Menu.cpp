/*
 * Menu.cpp
 *
 * Created: 2018/10/29 9:11:52
 *  Author: naitsuku
 */ 

#include "Menu.h"

/* ���ݑI�𒆃��j���[���� */
uint8_t menuSelectingItem = 0;

/* ���j���[�ݒ�l */
signed char menuItemSetValues[MENUITEM_MAX_NO+1];

/* ���j���[���ݐݒ蒆���ڂ̒l */
signed char * menuItemValue;

/* ���j���[���������� */
void menuInit(){
	uint8_t i;
	/* ���j���[�ݒ�l�������l�e�[�u������ǂݏo�� */
	for(i=0; i<MENUITEM_MAX_NO; i++){
		menuItemSetValues[i] = MENUITEM_DEFAULT_TBL[i];
	}
	menuSelectingItem = 0;
	menuItemValue = &menuItemSetValues[menuSelectingItem];
	
	/* ���j���[����\�� */
	menuRefreshDisplay();
}

/* ���j���[�\���X�V */
void menuRefreshDisplay(){
	char line0[20]; /* ��s�� */
	char line1[20]; /* ��s�� */
	char itemname[20];
	char valuename[20];
	menuItemValue = &menuItemSetValues[menuSelectingItem];
	uint8_t hasvaluename, vnaddr;
	
	hasvaluename = pgm_read_word(&MENUITEM_HAS_VALUENAME[menuSelectingItem]);
	strcpy_P(itemname, (char*)pgm_read_word(&(MENUITEMS_TBL[menuSelectingItem])));
	
	/*              0123456789ABCDEF*/
	sprintf(line0, "%s", itemname);
	
	/*  ���ݒ�l���j�[�N�������Ȃ��ꍇ */
	if(hasvaluename == DISABLE){
		sprintf(line1, "             %3d", *menuItemValue);
	}else{
		/* ���ݒ�l���j�[�N�������Ă���ꍇ */
		/* �ݒ�l���j�[�N���A�h���X���Z */
		vnaddr = *menuItemValue 
				 + pgm_read_word(&MENUITEM_VALUENAME_ADDR_TBL[menuSelectingItem]) 
				 - pgm_read_word(&MENUITEM_MIN_TBL[menuSelectingItem]);

		strcpy_P(valuename, (char*)pgm_read_word(&(MENUITEM_VALUENAME_TBL[vnaddr])));
		sprintf(line1, "         %s", valuename);
	}
	
	lcdSetCursor(0,0);
	lcdPrint(line0);
	
	lcdSetCursor(0,1);
	lcdPrint(line1);
	
}

/* ���j���[���ڈړ� */
void menuMoveItem(signed char arg){
	if( (arg==NEXT) && (menuSelectingItem<MENUITEM_MAX_NO) ){
		menuSelectingItem++;
	}
	else if( (arg==NEXT) && (menuSelectingItem==MENUITEM_MAX_NO) ){
		menuSelectingItem = 0;
	}
	else if( (arg==PREV) && (menuSelectingItem>0) ){
		menuSelectingItem--;
	}
	else if( (arg==PREV) && (menuSelectingItem==0) ){
		menuSelectingItem = MENUITEM_MAX_NO;
	}
	else{
		menuSelectingItem = 0;
	}
	
	menuItemValue = &menuItemSetValues[menuSelectingItem];
}

/* ���j���[�ݒ�l���� */
void menuAdjValue(signed char arg){
	signed char maxVal = pgm_read_word(&MENUITEM_MAX_TBL[menuSelectingItem]);
	signed char minVal = pgm_read_word(&MENUITEM_MIN_TBL[menuSelectingItem]);
	menuItemValue = &menuItemSetValues[menuSelectingItem];
	
	if( (arg==UP) && (*menuItemValue<maxVal) ){
		menuItemSetValues[menuSelectingItem]++;
	}
	else if( (arg==UP) && (*menuItemValue==maxVal) ){
		menuItemSetValues[menuSelectingItem] = minVal;
	}
	else if( (arg==DN) && (*menuItemValue>minVal) ){
		menuItemSetValues[menuSelectingItem]--;
	}
	else if( (arg==DN) && (*menuItemValue==minVal) ){
		menuItemSetValues[menuSelectingItem] = maxVal;
	}
}

/* ���j���[���̃A�N�e�B�r�e�B */
void menuActivity(){
	volatile uint8_t button;
	uint8_t changed = 1;
	button = buttonGetCommand();
	
	switch(button){
		case BTN_UP:
			/* �ް�����+ ���� */
			menuAdjValue(UP);
			break;
		case BTN_DN:
			/* �ް�����- ���� */
			menuAdjValue(DN);
			break;
		case BTN_PR:
			/* �O���� */
			menuMoveItem(PREV);
			break;
		case BTN_NX:
			/* ������ */
			menuMoveItem(NEXT);
			break;
		case BTN_OK:
			/* OK */
			break;
		case BTN_CA:
			/* �L�����Z�� */
			menuMoveItem(RST);
			break;
		default:
			changed = 0;
	}
	
	/* �{�^�����삪����ΐݒ蔽�f�E���j���[�\���X�V */
	if(changed == 1){
		/* ���j���[�C�x���g���s */
		if( (*MENUITEM_EVENT_TBL[menuSelectingItem]) != NULL){
			(*MENUITEM_EVENT_TBL[menuSelectingItem])();
		}
		/* ���j���[�\���X�V */
		menuRefreshDisplay();
	}
	
}

/* �� ���j���[�C�x���g */
void ev_setOctave(){
	setBOctave(*menuItemValue);
}

void ev_setTranspose(){
	setTranspose(*menuItemValue);
}

void ev_setPrisetTone(){
	setPrisetTone((uint8_t)*menuItemValue);
}