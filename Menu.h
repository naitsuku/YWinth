/*
 * Menu.h
 *
 * Created: 2018/10/29 9:12:10
 *  Author: naitsuku
 */ 


#ifndef MENU_H_
#define MENU_H_

#include "YWinthCommon.h"


/* メニュー動作定義 */
#define UP		 1
#define DN		-1
#define RST      0
#define NEXT	 1
#define PREV	-1
#define NOCNG	 0

/* メニュー項目番号最大値(メニュー項目数-1) */
#define MENUITEM_MAX_NO 5
#define CHR_SIZE 20

/* ボタンクリック時イベントポインタ */
typedef void (*EVENT)();

/* ★ メニュー項目名テーブル */
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

/* ★設定値ユニーク名テーブル */

/* 音色名ﾃｰﾌﾞﾙ */
const char TONENAME_STR_TBL[][8] PROGMEM = {
	"SinWave",
	"Brass 1",
	"Brass 2",
	"SquWave",
	"SawWave",
	"E.Piano",
	"Tnr.Sax"
};

/* 移調(トランスポーズ)テーブル */
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

/* 運指テーブル */
const char FINGERPTN_STR_TBL[4][8] PROGMEM = {
	" YWinth",
	"  Flute",
	"    Sax",
	"DigHorn"
};

/* 設定値ユニーク名ポインタテーブル */
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

/* 設定値ユニーク名保存位置アドレスオフセットテーブル */
const uint8_t MENUITEM_VALUENAME_ADDR_TBL[] PROGMEM = {
	255, /* MasterVolume */
	0,   /* PrisetTone */
	7,   /* Transpose */
	255,
	255,
	18
};


/* ★ メニュー変位テーブル */
/* めも：charが符号あるかは不定なので符号つけたいときは”signed char”とすること！ */

/* 設定可能最小値 */
const signed char MENUITEM_MIN_TBL[] PROGMEM = {
	0,		/* MasterVolume */
	0,		/* PrisetTone */
	-5,	/* Transpose */
	-4,		/* Octave */
	0,		/* BreathLevel */
	0		/* FingerPattern */
};

/* 設定可能最大値 */
const signed char MENUITEM_MAX_TBL[] PROGMEM = {
	31,		/* MasrerVolume */
	6,		/* PrisetTone */
	6,		/* Transpose */
	4,		/* Octave */
	100,	/* BreathLevel*/
	3		/* FingerPattern */
};

/* 初期値テーブル */
const signed char MENUITEM_DEFAULT_TBL[] PROGMEM = {
	20,		/* MasrerVolume */
	0,		/* PrisetTone */
	0,		/* Transpose */
	0,		/* Octave */
	50,		/* BreathLevel*/
	0		/* FingerPattern */
};

/* ★設定値ユニーク名保持の有無テーブル
	 設定値が数値:DISABLE 設定値が名前をもつ:ENABLE */
const signed char MENUITEM_HAS_VALUENAME[] PROGMEM = {
	DISABLE,		/* MasrerVolume */
	ENABLE,			/* PrisetTone */
	ENABLE,		/* Transpose */
	DISABLE,		/* Octave */
	DISABLE,		/* BreathLevel*/
	ENABLE			/* FingerPattern */
};


/* ★ メニューイベント */
void ev_setOctave();
void ev_setTranspose();
void ev_setPrisetTone();

/* メニューイベントテーブル：値編集時の動作 */
#if 1
const EVENT MENUITEM_EVENT_TBL[] = { /* memo... 関数ポインタはPROGMEMにしちゃいけないのかも？ */
	NULL,				/* MasrerVolume */
	ev_setPrisetTone,	/* PrisetTone */
	ev_setTranspose,	/* Transpose */
	ev_setOctave,		/* Octave */
	NULL,				/* BreathLevel*/
	NULL				/* FingerPattern */
};
#endif

/* メニューイベントテーブル：OKクリック時動作 */
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

/* メニューイベントテーブル：ｷｬﾝｾﾙクリック時動作 */
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

/* 現在選択中メニュー項目 */
extern uint8_t menuSelectingItem;

/* メニュー設定値 */
extern signed char menuItemSetValues[MENUITEM_MAX_NO+1];

/* 選択中項目の設定値 */
extern signed char * menuItemValue;

/* メニュー表示更新 */
void menuRefreshDisplay();

/* メニュー項目移動 */
void menuMoveItem(signed char arg);

/* メニュー設定値調整 */
void menuAdjValue(signed char arg);

/* メニュー一回のアクティビティ */
void menuActivity();

/* メニュー初期化処理 */
void menuInit();


#endif /* MENU_H_ */