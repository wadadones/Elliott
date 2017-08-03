// StraightPhase.cpp

#include "StraightPhase.hpp"

#define LIGHT_WHITE  40
/* ���F�̌��Z���T�l */
#define LIGHT_BLACK  0
/* �o�����X���s���̊p�x[�x] */
#define TAIL_ANGLE_DRIVE      3

StraightPhase::StraightPhase(std::string name, Balancer* balancer) : Phase(name, balancer)
{
	/*�R���X�g���N�^*/;
}

void StraightPhase::initialize()
{
	/*��������*/;
}

void StraightPhase::calculate(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char* pwm_L, signed char* pwm_R, signed int* ang_T)
{
	/*�Z���T����̓��͂����ƂɃ��[�^�ւ̏o�͂��v�Z����*/
	/*turn = 1��2�l����*/
	signed char forward;
	signed char turn;
	forward = 100;
	if (color >= (LIGHT_WHITE + LIGHT_BLACK) / 2)
	{
		turn =  -1; /* �E���񖽗� */
	}
	else
	{
		turn = 1; /* �����񖽗� */
	}
	balancer->setCommand(forward, turn);
	balancer->update(gyro, motor_ang_r, motor_ang_l, volt);
	*pwm_L = balancer->getPwmRight();
	*pwm_R = balancer->getPwmLeft();
	*ang_T = TAIL_ANGLE_DRIVE;
}

bool StraightPhase::isFinished(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char pwm_L, signed char pwm_R, signed int ang_T)
{
	/*�I�������𖞂�����true,�������Ȃ����false*/
	/*6�b�ŏI��*/
	if (time <= 6000)
		return false;
	else
		return true;
}
