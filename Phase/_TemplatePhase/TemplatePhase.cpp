// /*TemplatePhase*/.cpp

#include "/*TemplatePhase*/.hpp"

/*TemplatePhase*/::/*TemplatePhase*/(std::string name, Balancer* balancer) : Phase(name, balancer)
{
	/*�R���X�g���N�^*/;
}

void /*TemplatePhase*/::initialize()
{
	/*��������*/;
}

void /*TemplatePhase*/::calculate(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char* pwm_L, signed char* pwm_R, signed int* ang_T)
{
	/*�Z���T����̓��͂����ƂɃ��[�^�ւ̏o�͂��v�Z����*/
	*pwm_L = 0;
	*pwm_R = 0;
	*ang_T = 0;
}

bool /*TemplatePhase*/::isFinished(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char pwm_L, signed char pwm_R, signed int ang_T)
{
	/*�I�������𖞂�����true,�������Ȃ����false*/
	if(1)
		return false;
	else
		return true;
}
