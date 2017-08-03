// LineTracer.cpp

#include "LineTracer.hpp"

/* 白色の光センサ値 */
#define LIGHT_WHITE  40
/* 黒色の光センサ値 */
#define LIGHT_BLACK  0

/* バランス走行時の角度[度] */
#define TAIL_ANGLE_DRIVE      3

LineTracer::LineTracer(std::string name, Balancer* balancer) : Phase(name, balancer)
{
}

void LineTracer::initialize()
{
	;
}

void LineTracer::calculate(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char* pwm_L, signed char* pwm_R, signed int* ang_T)
{
	signed char forward;
	signed char turn;
	forward = 30;
	if (color >= (LIGHT_WHITE + LIGHT_BLACK)/2){
		turn =  20;
	}else{
		turn = -20;
	}
	balancer->setCommand(forward, turn);
	balancer->update(gyro, motor_ang_r, motor_ang_l, volt);
	*pwm_L = balancer->getPwmRight();
	*pwm_R = balancer->getPwmLeft();
	*ang_T = TAIL_ANGLE_DRIVE;
}

bool LineTracer::isFinished(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char pwm_L, signed char pwm_R, signed int ang_T)
{
	if(time <= 60000)
		return false;
	else
		return true;
}
