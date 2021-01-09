/*
 * Midi.h
 *
 * Created: 2018/10/31 23:06:09
 *  Author: naitsuku
 */ 


#ifndef MIDI_H_
#define MIDI_H_

#include "YWinthCommon.h"

#define MIDI_ENABLE 1

/* MIDIボーレート：31250bps */
#define MIDIBAUD 31250

#define MIDI_NOTEON 0x90 /* ノートオン */
#define MIDI_NOTEOF 0x80 /* ノートオフ */
#define MIDI_CHPRSH 0xD0 /* チャンネルプレッシャー */


/* MIDI初期化 */
void midiInit();

/* MIDIノートオン */
void midiNoteOn(uint8_t ch, uint8_t note, uint8_t vel);

/* MIDIノートオフ */
void midiNoteOff(uint8_t ch, uint8_t note);

/* MIDIオールノートオフ */
void midiAllNoteOff(uint8_t ch);

/* MIDIアフタータッチ */
void midiAfterTouch(uint8_t ch, uint8_t vel);

/* MIDIコントロールチェンジ */
void midiControlChange();

/* MIDIプログラムチェンジ */
void midiProgramChange();

void midiPutc(uint8_t a);

void midiPuts(uint8_t * str);

#endif /* MIDI_H_ */