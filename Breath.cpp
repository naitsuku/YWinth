/*
 * Breath.cpp
 *
 * Created: 2018/10/24 8:51:14
 *  Author: naitsuku
 */ 

#include "Breath.h"

uint32_t offset;

/* I2C�֘A */
void i2cInit(void)
{
	TWBR = 2;
	TWSR = 0x08;
	TWCR = (1<<TWEN); /* I2C�L���� */
}

void i2cStart(void)
{
	/* I2C�J�n�������s */
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	/* ���M�����҂� */
	while(!(TWCR & 1<<TWINT));
}

void i2cStop(void)
{
	/* I2C��~�������s */
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void i2cWrite(int data)
{
	/* ���M�f�[�^ */
	TWDR = data;
	/* ���M�J�n */
	TWCR = (1<<TWINT)|(1<<TWEN);
	/* �����҂� */
	while(!(TWCR & 1<<TWINT));
}

int i2cRead(int i)
{
	if(i==0)//ACK�@�����ēǂݍ���
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	}
	else//NOACK �ǂݍ��ݍŌ�
	{
		TWCR = (1<<TWINT)|(1<<TWEN);
	}
	while(!(TWCR & 1<<TWINT)){}
	return TWDR;
}

int i2cRegRead(uint8_t addr,uint8_t reg)
{
	uint8_t i2c_data;
	/* i2c�J�n�E�X���[�u�A�h���X���M�E���W�X�^�A�h���X���M */
	i2cInit();
	i2cStart();
	i2cWrite( (addr<<1)+0 ); /* �X���[�u�A�h���X���M(RW=0) */
	i2cWrite(reg);  /* ���W�X�^�A�h���X�������� */
	
	/* �X���[�u�A�h���XLSB��R/W�ɂȂ�I */
	
	/* ���W�X�^�ǂݍ��݁Ei2c�ʐM�I�� */
	i2cStart();
	i2cWrite( (addr<<1)+1 ); /* �X���[�u�A�h���X���M(RW=1) */
	i2c_data = i2cRead(1);  /* ���W�X�^�ǂݍ��� */
	i2cStop();
	
	return i2c_data;
}

void i2cRegWrite(uint8_t addr, uint8_t reg, uint8_t data){
	/* i2c�J�n�E�X���[�u�A�h���X���M�E���W�X�^�A�h���X���M */
	i2cInit();
	i2cStart();
	i2cWrite( (addr<<1)+0 ); /* �X���[�u�A�h���X���M(RW=0) */
	i2cWrite(reg);  /* ���W�X�^�A�h���X�������� */
	i2cWrite(data);		/* ���W�X�^�������� */
	i2cStop();
}

/* �u���X�Z���T�C���l�擾 */
uint32_t breathRead(){
	uint32_t breath;
	uint8_t pll, pl, ph;
	pll = i2cRegRead(LPS22_ADDR, LPS22_PLL);
	pl  = i2cRegRead(LPS22_ADDR, LPS22_PL);
	ph  = i2cRegRead(LPS22_ADDR, LPS22_PH);
	
	breath   = ph;
	breath <<= 8;
	breath  += pl;
	breath <<= 8;
	breath  += pll;
	
	return breath;
}

/* �u���X�Z���T������ */
void breathInit(){
	i2cRegWrite(LPS22_ADDR, LPS22_CTR1,
				0b01010000);
	/*			  |||||||+- SIM
				  ||||||+-- BDU
				  |||||+--- LPFP_CFG
				  ||||+---- EN_LPFP
				  |||+----- ODR0   Output Data Rate
				  ||+------ ODR1   000:Disable, 001:1Hz, 010:10Hz
				  |+------- ODR2   011:25Hz, 100:50Hz, 101:75Hz
				  +-------- "0"
	*/
}

void setBreathOffset(){
	offset = breathRead();
}

long getBreathOffsetValue(){
	uint32_t enOfstBreath;
	enOfstBreath = breathRead() - offset;
	return enOfstBreath;
}

/* �I�t�Z�b�g�ς݃u���X�l(0�`20000)�˃x���V�e�B�l(0~127) */
uint8_t breathToVelocity(long brth_lv){
	uint8_t velocity;
	/* �͈͊O�̏ꍇ�̏��� */
	if(brth_lv < 0){
		velocity = 0;
	}else if(brth_lv > 12700){
		velocity = 127;
	}else{
		velocity = brth_lv / 100;
	}
	return velocity;
}

uint8_t breathToVovol(long brth_lv){
	return breathToVelocity(brth_lv)/4;
}

