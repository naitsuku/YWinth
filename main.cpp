/*
 * ウィンドシンセサイザー製作プロジェクト『Ywinth』
 * YWinth_MainMPU.cpp
 *
 * Created: 2018/10/02 10:39:11
 * Author : naitsuku
 */ 

using namespace std;

/* 共通ライブラリ */
#include "YWinthCommon.h"

/* グローバル変数 */
volatile uint8_t ledstate=0;
volatile uint8_t cnt=0;
volatile bool executePermission = ENABLE;


/* タイマ割込み発生時処理
   約 1000Hz T=1mS */
ISR (TIMER0_COMPA_vect){
	/* 120Hz 8mSに一度メイン処理実行 */
	if(cnt == 3){
		/* メイン処理実行許可発行 */
		executePermission = ENABLE; /* 実行許可の開放はメイン処理で行う */
		cnt = 0;
	}else{
		cnt++;
	}
	/* チャタリング除去処理 4mS一致でボタン値確定 */
	buttonSampling();		/* 1.サンプリング */
	buttonAveraging();		/* 2.平均化 */
	buttonPressDetect();	/* 3.押下検知 */
}

/* USART0受信割り込み */
ISR (USART0_RX_vect){
	RxBuff[RxBuffWrPt] = UDR0; /* バッファに積む */
	RxBuffWrPt++;
}

/* メイン処理 */
int main(void)
{
	/* メイン処理：初期化部分 */
	char str[100];
	uint32_t data;
	long bdata;
	uint8_t keyval;
	uint16_t noteNum, noteNum_old=0;
	char transpose=12;
	uint8_t vel, vel_old=0;
	
	spiInit();
	spiCtrlCs(DISABLE);
	
	setIOMode();
	buttonInit();
	//uartInit();
	
	soundInit();
	setTone();
	setCh();
	
	keyOnNoteNoWithVovol(60, 31);
	
	uartInit();
	
	/* UARTスタートアップﾒｯｾｰｼﾞ送信 */
	sprintf(str, "***      YWinth Serial Console      ***\nFirmware version: %s\n", VERSIONCODE);
	uartPuts(str);
	uartPuts("4 Operator FM Sound Wind Synthesizer.\n");
	uartPuts("Designed by K.Yazawa.\n");
	uartPuts("This is Serial Console.\n");
	uartPuts("Please input command.\n");

	/* MIDI初期化処理 */
	midiInit();

	/* LCD表示処理 LCDは遅いので気を遣う */
	_delay_ms(100);
	lcdInit();
	_delay_ms(100);
	lcdPrint("YWinth");
	_delay_ms(20);
	lcdSetCursor(11,1);
	_delay_ms(20);
	lcdPrint(VERSIONCODE);
	_delay_ms(20);
	
	keyOff();
	
	initTimer();
	breathInit();
	
	cnt = 0;
	
	_delay_ms(500); /* 息安定待ち */
	setBreathOffset();
	_delay_ms(500);
	midiAllNoteOff(0x01);
	
	menuInit();
	/* 初期化完了，割り込み有効化！ */
	sei();
	
	/* メイン処理：ループ部分 */
	
	while(1){
		
		/* タイマにて8mS周期で実行許可発行 */
		/* 実行許可確認し実行可能なら実行 */
		while(executePermission == DISABLE);
		
		/* ★★★ 演奏処理 ★★★ */
			
		/* ブレスデータ取得 ⇒ ベロシティ生成 */
		bdata = getBreathOffsetValue();
		data = breathToVovol(bdata);
		vel = breathToVelocity(bdata);
	
		
		/* タッチセンサデータ取得 ⇒ ノートナンバー生成 */
		keyval = touchGet();
		noteNum = fingerToNoteNum(keyval);
			
		/* ノートナンバー＋ベロシティ ⇒ キーオン */
		if(noteNum != 0){
			keyOnNoteNoWithVovol(noteNum, data);
		}else{
			keyOff();
		}
		
		uartGetCmdStr();
		sprintf(str, "%d\n", data);
		//uartPuts(str);
		
		/* MIDI送信処理 */
#if 1   /* なんかうまくいかない... ハードウェアのデバッグしないといけない気がする */
		/*息終了⇒ノートオフ */
		if( (vel<20) ){ 
			if(vel_old>20){
				midiNoteOn(0x01, noteNum_old, 0);
				keyOff();
			}
		}else
		if( ((vel_old<20)&&(vel>=20)) ){ /* 息開始でノートオン */
			midiNoteOn(0x01, noteNum, vel);
			//keyOnNoteNoWithVovol(noteNum, data);
		}else{
			//midiAfterTouch(0x01, vel);
			//keyOnNoteNoWithVovol(noteNum, data);
		}
		
		//_delay_ms(30);
		
		
		/* めも：ノートオン検出の処理は関数などつかってもう少しまとめたい
				ブレスにヒステリシスがあるとよい気がする */
#endif

/*
		midiNoteOn(0x00,60, 0x50);
		_delay_ms(200);
		//midiNoteOff(0x01,60);
		midiNoteOn(0x00,60, 0x00);
		_delay_ms(200);
*/
		vel_old = vel;
		noteNum_old = noteNum;
		
		/* メニュー処理 */
		menuActivity();
			
		/* 実行許可解放 */
		executePermission = DISABLE;
		
	}
}

// PIN13(ARD) = PB5
void setIOMode(void){
	DDRB |= 0b00100000;
}

void ctrlLed(uint8_t state){
	uint8_t reg;
	reg = PORTB;
	reg &= 0b11001111;
	reg |= state << 5;
	PORTB = reg;
}


void initTimer(void)
{
	/* Timer0: 8bitタイマ設定 */
	TCCR0A = 0b00000010; /* タイマ制御レジスタＡ */
	
	/* CTC動作！ */
	TCCR0B = 0b00000101; /* タイマ制御レジスタＢ */
	/*              ||+- CS00 ｸﾛｯｸｿｰｽ選択
                    |+-- CS01 101 : 1024分周 
					+--- CS02 16M/1024 = 15.625KHz
					タイマ出力 61.04Hz T=16.384mS
	*/
	
	OCR0A = 16; /* コンペア対象(255の場合 標準動作と同じ) */
	/* 約1mS周期 */
	
	TIMSK0 = 0b00000010; /* タイマ割込マスクレジスタ */
	/*               +- OCIE0A ﾀｲﾏ ｱｳﾄﾌﾟｯﾄｺﾝﾍﾟｱﾏｯﾁA割込有効
	*/
	
}



