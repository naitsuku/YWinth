/*
 * �E�B���h�V���Z�T�C�U�[����v���W�F�N�g�wYwinth�x
 * YMF825.h
 *
 * Created: 2018/10/02 10:39:11
 * Author : naitsuku
 */ 

#ifndef _YMF825_H_
#define _YMF825_H_

using namespace std;

#include "YWinthCommon.h"

/* YMF825����֘A */
void soundInit();
void setTone();
void keyOn(uint8_t fnumh, uint8_t fnuml, uint8_t vovol);
void keyOff(void);
void setCh(void);

/* �m�[�g�i���o��FNUM�ϊ��֘A */
uint8_t noteNoToScale(uint16_t noteNo);
uint8_t noteNoToBlock(uint16_t noteNo);
uint16_t noteNoToFnum(uint16_t noteNo);
void keyOnNoteNo(uint16_t noteNo);
void keyOnNoteNoWithVovol(uint16_t noteNo, uint8_t vovol);

#endif