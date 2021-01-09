/*
 * Breath.cpp
 *
 * Created: 2018/10/24 8:51:14
 *  Author: naitsuku
 */ 

#include "Breath.h"

uint32_t offset;

/* I2C関連 */
void i2cInit(void)
{
	TWBR = 2;
	TWSR = 0x08;
	TWCR = (1<<TWEN); /* I2C有効化 */
}

void i2cStart(void)
{
	/* I2C開始条件発行 */
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	/* 送信完了待ち */
	while(!(TWCR & 1<<TWINT));
}

void i2cStop(void)
{
	/* I2C停止条件発行 */
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void i2cWrite(int data)
{
	/* 送信データ */
	TWDR = data;
	/* 送信開始 */
	TWCR = (1<<TWINT)|(1<<TWEN);
	/* 完了待ち */
	while(!(TWCR & 1<<TWINT));
}

int i2cRead(int i)
{
	if(i==0)//ACK　続けて読み込み
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	}
	else//NOACK 読み込み最後
	{
		TWCR = (1<<TWINT)|(1<<TWEN);
	}
	while(!(TWCR & 1<<TWINT)){}
	return TWDR;
}

int i2cRegRead(uint8_t addr,uint8_t reg)
{
	uint8_t i2c_data;
	/* i2c開始・スレーブアドレス送信・レジスタアドレス送信 */
	i2cInit();
	i2cStart();
	i2cWrite( (addr<<1)+0 ); /* スレーブアドレス送信(RW=0) */
	i2cWrite(reg);  /* レジスタアドレス書き込み */
	
	/* スレーブアドレスLSBがR/Wになる！ */
	
	/* レジスタ読み込み・i2c通信終了 */
	i2cStart();
	i2cWrite( (addr<<1)+1 ); /* スレーブアドレス送信(RW=1) */
	i2c_data = i2cRead(1);  /* レジスタ読み込み */
	i2cStop();
	
	return i2c_data;
}

void i2cRegWrite(uint8_t addr, uint8_t reg, uint8_t data){
	/* i2c開始・スレーブアドレス送信・レジスタアドレス送信 */
	i2cInit();
	i2cStart();
	i2cWrite( (addr<<1)+0 ); /* スレーブアドレス送信(RW=0) */
	i2cWrite(reg);  /* レジスタアドレス書き込み */
	i2cWrite(data);		/* レジスタ書き込み */
	i2cStop();
}

/* ブレスセンサ気圧値取得 */
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

/* ブレスセンサ初期化 */
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

/* オフセット済みブレス値(0～20000)⇒ベロシティ値(0~127) */
uint8_t breathToVelocity(long brth_lv){
	uint8_t velocity;
	/* 範囲外の場合の処理 */
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

