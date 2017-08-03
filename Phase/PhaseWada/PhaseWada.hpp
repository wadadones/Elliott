// PhaseWada.hpp

#ifndef PHASE_H_WADA
#define PHASE_H_WADA
#include "../Phase.hpp"
#include <string>

class PhaseWada: public Phase {
public:
	PhaseWada(std::string name, Balancer* balancer);
	void initialize();
	void calculate(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char* pwm_L, signed char* pwm_R, signed int* ang_T);
	bool isFinished(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char pwm_L, signed char pwm_R, signed int ang_T);
	int p_control(int color);
	int pid_control(float* diff_sum, float* diff_prev, int color, int time);
};

#endif
