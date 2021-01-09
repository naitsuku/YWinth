/*
 * Finger.cpp
 *
 * Created: 2018/10/24 23:16:00
 *  Author: naitsuku
 */ 

#include "Finger.h"

signed char BOctave = 0;
signed char TransposeValue = 0;

uint16_t fingerToNoteNum(uint8_t finger){
	uint16_t noteNum;
	//noteNum = FINGER_TBL[fingerI];
	noteNum = pgm_read_word(&FINGER_TBL[finger]);
	/* PROGMEMﾃｰﾌﾞﾙ参照時は，?の関数を読んで読み出す！！
	   配列として読むと変な値が出るので注意する！ */
	if(noteNum == 0){
		//noteNum = pgm_read_word(&FINGER_TBL[0]); /* 未定義の運指の場合は C# を出す */
	}else{
		/* ノートナンバーに基準オクターブ反映 */
		noteNum += (BOctave*12);
		/* トランスポーズ値反映 */
		noteNum += TransposeValue;
	}
	return noteNum;
}

void setBOctave(signed char oct){
	BOctave = oct;
}

void setTranspose(signed char transpose){
	TransposeValue = transpose;
}