/*
 * ウィンドシンセサイザー製作プロジェクト『Ywinth』
 * YMF825.h
 *
 * Created: 2018/10/02 10:39:11
 * Author : naitsuku
 */ 

#ifndef _YMF825_H_
#define _YMF825_H_

using namespace std;

#include "YWinthCommon.h"

/* YMF825制御関連 */
void soundInit();
void setTone();
void keyOn(uint8_t fnumh, uint8_t fnuml, uint8_t vovol);
void keyOff(void);
void setCh(void);

/* ノートナンバ⇒FNUM変換関連 */
uint8_t noteNoToScale(uint16_t noteNo);
uint8_t noteNoToBlock(uint16_t noteNo);
uint16_t noteNoToFnum(uint16_t noteNo);
void keyOnNoteNo(uint16_t noteNo);
void keyOnNoteNoWithVovol(uint16_t noteNo, uint8_t vovol);

#endif