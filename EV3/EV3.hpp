// EV3.h

#ifndef EV3_H
#define EV3_H

#include "ev3api.h"

class EV3 {
private:
	void ev3_tail_set_angle(signed int angle);
public:
	void setUpEV3();
	void waitForStart();
	void resetSensorValue();
	void playTone();
	void getSensorValue(int32_t* motor_ang_l, int32_t* motor_ang_r, int* color, int* gyro, int* sonic, int* volt);
	void setMotorValue(signed char pwm_L, signed char pwm_R, signed int ang_T);
	void sleep(int sleep_sec);
	bool buttonIsPressed();
	void shutDownEV3();
	FILE* getBT();
	void setBTcmd(int cmd);
};

#endif
