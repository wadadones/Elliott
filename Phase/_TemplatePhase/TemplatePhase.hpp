// /*TemplatePhase*/.hpp

#ifndef /*TEMPLATE_PHASE_H*/
#define /*TEMPLATE_PHASE_H*/

#include "../Phase.hpp"
#include <string>

class /*TemplatePhase*/: public Phase {
public:
	/*TemplatePhase*/(std::string name, Balancer* balancer);
	void initialize();
	void calculate(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char* pwm_L, signed char* pwm_R, signed int* ang_T);
	bool isFinished(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char pwm_L, signed char pwm_R, signed int ang_T);
};

#endif
