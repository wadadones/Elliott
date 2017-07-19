// Phase.h

#ifndef PHASE_H
#define PHASE_H

#include "../Balancer/BalancerCPP.hpp"
#include <string>

class Phase {
protected: 
	std::string name;
	Balancer* balancer;
public:
	Phase(std::string name, Balancer* balancer);
	std::string getName();
	virtual void initialize() = 0;
	virtual void calculate(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char* pwm_L, signed char* pwm_R, signed int* ang_T) = 0;
	virtual bool isFinished(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char pwm_L, signed char pwm_R, signed int ang_T) = 0;
};

#endif
