// StraightPhase.cpp

#include "StraightPhase.hpp"

#define LIGHT_WHITE  40
/* 黒色の光センサ値 */
#define LIGHT_BLACK  0
/* バランス走行時の角度[度] */
#define TAIL_ANGLE_DRIVE      3

StraightPhase::StraightPhase(std::string name, Balancer* balancer) : Phase(name, balancer)
{
	/*コンストラクタ*/;
}

void StraightPhase::initialize()
{
	/*初期処理*/;
}

void StraightPhase::calculate(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char* pwm_L, signed char* pwm_R, signed int* ang_T)
{
	/*センサからの入力をもとにモータへの出力を計算する*/
	/*turn = 1の2値制御*/
	signed char forward;
	signed char turn;
	forward = 100;
	if (color >= (LIGHT_WHITE + LIGHT_BLACK) / 2)
	{
		turn =  -1; /* 右旋回命令 */
	}
	else
	{
		turn = 1; /* 左旋回命令 */
	}
	balancer->setCommand(forward, turn);
	balancer->update(gyro, motor_ang_r, motor_ang_l, volt);
	*pwm_L = balancer->getPwmRight();
	*pwm_R = balancer->getPwmLeft();
	*ang_T = TAIL_ANGLE_DRIVE;
}

bool StraightPhase::isFinished(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char pwm_L, signed char pwm_R, signed int ang_T)
{
	/*終了条件を満たせばtrue,満たさなければfalse*/
	/*6秒で終了*/
	if (time <= 6000)
		return false;
	else
		return true;
}
