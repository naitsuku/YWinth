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
	/* PROGMEMð��َQ�Ǝ��́C?�̊֐���ǂ�œǂݏo���I�I
	   �z��Ƃ��ēǂނƕςȒl���o��̂Œ��ӂ���I */
	if(noteNum == 0){
		//noteNum = pgm_read_word(&FINGER_TBL[0]); /* ����`�̉^�w�̏ꍇ�� C# ���o�� */
	}else{
		/* �m�[�g�i���o�[�Ɋ�I�N�^�[�u���f */
		noteNum += (BOctave*12);
		/* �g�����X�|�[�Y�l���f */
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