/*
 * Menu.cpp
 *
 * Created: 2018/10/29 9:11:52
 *  Author: naitsuku
 */ 

#include "Menu.h"

/* 現在選択中メニュー項目 */
uint8_t menuSelectingItem = 0;

/* メニュー設定値 */
signed char menuItemSetValues[MENUITEM_MAX_NO+1];

/* メニュー現在設定中項目の値 */
signed char * menuItemValue;

/* メニュー初期化処理 */
void menuInit(){
	uint8_t i;
	/* メニュー設定値を初期値テーブルから読み出し */
	for(i=0; i<MENUITEM_MAX_NO; i++){
		menuItemSetValues[i] = MENUITEM_DEFAULT_TBL[i];
	}
	menuSelectingItem = 0;
	menuItemValue = &menuItemSetValues[menuSelectingItem];
	
	/* メニュー初回表示 */
	menuRefreshDisplay();
}

/* メニュー表示更新 */
void menuRefreshDisplay(){
	char line0[20]; /* 一行目 */
	char line1[20]; /* 二行目 */
	char itemname[20];
	char valuename[20];
	menuItemValue = &menuItemSetValues[menuSelectingItem];
	uint8_t hasvaluename, vnaddr;
	
	hasvaluename = pgm_read_word(&MENUITEM_HAS_VALUENAME[menuSelectingItem]);
	strcpy_P(itemname, (char*)pgm_read_word(&(MENUITEMS_TBL[menuSelectingItem])));
	
	/*              0123456789ABCDEF*/
	sprintf(line0, "%s", itemname);
	
	/*  ★設定値ユニーク名持たない場合 */
	if(hasvaluename == DISABLE){
		sprintf(line1, "             %3d", *menuItemValue);
	}else{
		/* ★設定値ユニーク名持っている場合 */
		/* 設定値ユニーク名アドレス演算 */
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

/* メニュー項目移動 */
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

/* メニュー設定値調整 */
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

/* メニュー一回のアクティビティ */
void menuActivity(){
	volatile uint8_t button;
	uint8_t changed = 1;
	button = buttonGetCommand();
	
	switch(button){
		case BTN_UP:
			/* ﾃﾞｰﾀｴﾝﾄﾘ+ 処理 */
			menuAdjValue(UP);
			break;
		case BTN_DN:
			/* ﾃﾞｰﾀｴﾝﾄﾘ- 処理 */
			menuAdjValue(DN);
			break;
		case BTN_PR:
			/* 前項目 */
			menuMoveItem(PREV);
			break;
		case BTN_NX:
			/* 次項目 */
			menuMoveItem(NEXT);
			break;
		case BTN_OK:
			/* OK */
			break;
		case BTN_CA:
			/* キャンセル */
			menuMoveItem(RST);
			break;
		default:
			changed = 0;
	}
	
	/* ボタン操作があれば設定反映・メニュー表示更新 */
	if(changed == 1){
		/* メニューイベント実行 */
		if( (*MENUITEM_EVENT_TBL[menuSelectingItem]) != NULL){
			(*MENUITEM_EVENT_TBL[menuSelectingItem])();
		}
		/* メニュー表示更新 */
		menuRefreshDisplay();
	}
	
}

/* ★ メニューイベント */
void ev_setOctave(){
	setBOctave(*menuItemValue);
}

void ev_setTranspose(){
	setTranspose(*menuItemValue);
}

void ev_setPrisetTone(){
	setPrisetTone((uint8_t)*menuItemValue);
}