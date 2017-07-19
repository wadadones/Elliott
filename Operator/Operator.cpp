// Operator.cpp

#include "Operator.hpp"

//カウントアップ時間
#define COUNT_TIME 4
//タスク停止時間
#define SLEEP_TIME 3

//コンストラクタ
Operator::Operator(EV3* ev3, Balancer* balancer, Schedule* schedule)
{
	this->ev3 = ev3;
	this->balancer = balancer;
	this->schedule = schedule;
}

void Operator::run()
{
	//実行中フェーズのポインタ
	Phase* currentPhase;
	//各フェーズの経過時間
	int time;
	//センサからの入力
	int32_t motor_ang_l;
	int32_t motor_ang_r;
	int color;
	int gyro;
	int sonic;
	int volt;
	//モータへの出力
	signed char pwm_L;
	signed char pwm_R;
	signed int ang_T;

	//EV3の設定
	ev3->setUpEV3();
	//スタート指示待ち
	ev3->waitForStart();
	//センサのリセット
	ev3->resetSensorValue();
	//倒立走行ライブラリの初期化
	balancer->init();

	while(schedule->isEmpty()){
		//次のフェーズを取り出す
		currentPhase = schedule->popPhase();
		//フェーズの初期処理
		currentPhase->initialize();
		//タイマーのリセット
		time = 0;
		//スタート通知
		ev3->playTone();

		do{
			//センサから入力を貰う	
			ev3->getSensorValue(&motor_ang_l, &motor_ang_r, &color, &gyro, &sonic, &volt);
			//入力値から出力値をフェーズごとの方法で算出
			currentPhase->calculate(time, motor_ang_l, motor_ang_r, color, gyro, sonic, volt, &pwm_L, &pwm_R, &ang_T);
			//モータに出力を渡す
			ev3->setMotorValue(pwm_L, pwm_R, ang_T);
			//おやすみ
			ev3->sleep(SLEEP_TIME);
			//タイマーのインクリメント
			time += COUNT_TIME;
			//左上のボタンで終了
			if(ev3->buttonIsPressed()) break;
		}while(!currentPhase->isFinished(time, motor_ang_l, motor_ang_r, color, gyro, sonic, volt, pwm_L, pwm_R, ang_T));

		//左上のボタンでry
		if(ev3->buttonIsPressed()) break;
	}

	//終了通知
	ev3->playTone();
	//EV3の終了処理
	ev3->shutDownEV3();

	return;
}
