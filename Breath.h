/*
 * Breath.h
 *
 * Created: 2018/10/24 8:51:28
 *  Author: naitsuku
 */ 


#ifndef BREATH_H_
#define BREATH_H_

#include "YWinthCommon.h"

/* LPS22HB �X���[�u�A�h���X */
#define LPS22_ADDR	0b1011100

/* LPS22HB ���W�X�^�A�h���X */
#define LPS22_WAMI  0x0F /* LPS22HB ���A�h���X�ԓ�(Who am I) */
#define LPS22_PLL	0x28 /* LPS22HB �C���l 0-7 */
#define LPS22_PL	0x29 /* LPS22HB �C���l 8-15 */
#define LPS22_PH	0x2A /* LPS22HB �C���l 16-23 */
#define LPS22_CTR1	0x10 /* LPS22HB �R���g���[�����W�X�^ */

/* �O���[�o���ϐ� */
extern uint32_t offset; /* �C���I�t�Z�b�g�l(�O���[�o���ϐ�) */

/* I2C����֘A */
void i2cInit(void);
void i2cStart(void);
void i2cStop(void);
void i2cWrite(int data);
int i2cRead(int i);
int i2cRegRead(uint8_t addr,uint8_t reg);
void i2cRegWrite(uint8_t addr, uint8_t reg, uint8_t data);

/* �u���X�Z���T������ */
void breathInit();
/* �u���X�Z���T����C���l�ǂݎ�� */
uint32_t breathRead();
/* ���݋C���l��̾�Ēl�ɐݒ� */
void setBreathOffset();
/* �̾�čςݒl��Ԃ� */
long getBreathOffsetValue();
/*  �I�t�Z�b�g�ς݃u���X�l(0�`20000)�˃x���V�e�B�l(0~127) */
uint8_t breathToVelocity(long brth_lv);
uint8_t breathToVovol(long brth_lv);

#endif /* BREATH_H_ */