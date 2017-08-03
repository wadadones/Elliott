// PhaseWada.cpp

#include "PhaseWada.hpp"

#define LIGHT_WHITE  40
/* 黒色の光センサ値 */
#define LIGHT_BLACK  0
/* バランス走行時の角度[度] */
#define TAIL_ANGLE_DRIVE      3

FILE *fp;

PhaseWada::PhaseWada(std::string name, Balancer* balancer) : Phase(name, balancer)
{
	/*コンストラクタ*/;
}

void PhaseWada::initialize()
{
	/*初期処理(pidログゲット)*/
	fp=fopen("logwada.csv","w");
    if(fp==NULL){
        printf("failed\n");
        return;
    }
	fprintf(fp,"time,turnp,turni,turnd,turn\n");
}

void PhaseWada::calculate(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char* pwm_L, signed char* pwm_R, signed int* ang_T)
{
	/*センサからの入力をもとにモータへの出力を計算する*/
	signed char forward;
	signed char turn;
	float diff_sum = 0.0;
	float diff_prev = 0.0;
	forward = 90;
	//turn = p_control(color);
	turn = pid_control(&diff_sum, &diff_prev, color, time); 
	balancer->setCommand(forward, turn);
	balancer->update(gyro, motor_ang_r, motor_ang_l, volt);
	*pwm_L = balancer->getPwmRight();
	*pwm_R = balancer->getPwmLeft();
	*ang_T = TAIL_ANGLE_DRIVE;
	
}

bool PhaseWada::isFinished(int time, int32_t motor_ang_l, int32_t motor_ang_r, int color, int gyro, int sonic, int volt, signed char pwm_L, signed char pwm_R, signed int ang_T)
{
	/*終了条件を満たせばtrue,満たさなければfalse*/
	/*40秒で終了*/
	if (time <= 40 * 1000)
		return false;
	else
		return true;
}

/*P制御*/
/*int PhaseWada::p_control(int color)
{
	const float Kp = 0.83;
	const int target = (LIGHT_WHITE + LIGHT_BLACK) / 2;
	const int bias = 0;
	int diff = color - target;
	float turn = Kp * diff + bias;
	return turn;
}*/

/*PID制御*/
int PhaseWada::pid_control(float* diff_sum, float* diff_prev, int color, int time)
{
	const float Kp = 0.8;//1.43, 0.01, 1.50
	const float Ki = 0.8;//0.1
	const float Kd = 1.50;
	const int target = (LIGHT_WHITE + LIGHT_BLACK) / 2;
	const int bias = 0;


	float diffp = color - target;
	if (diffp > 20.0) diffp = 20.0;
	else if (diffp < -20.0) diffp = -20.0;
	float diffi = (diffp / 2.0 * 0.004) + (*diff_sum);
	float diffd = (diffp - (*diff_prev)) / 2.0 * 0.004;
	//int sum = diffp + diffd;
	//fprintf(fp,"ratep = %f, rated = %f\n", (double)diffp / sum, (double)diffd / sum);
	(*diff_sum) += ((*diff_prev) + diffp) / 2.0 * 0.004;
	(*diff_prev) = diffp;
	float turnp = Kp * diffp + bias;
	float turni = Ki * diffi;
	float turnd = Kd * diffd;

	/*修正前pid制御*/
	/*if (turnd > 10.0) {
		turnd = 10.0;
	}
	if (turnd < -10.0) {
		turnd = -10.0;
	}*/
	/*int diffp = color - target;
	if (diffp > 20.0)diffp = 20.0;
	else if (diffp < -20.0)diffp = -20.0;
	int diffi = diffp + (*diff_sum);
	int diffd = diffp - (*diff_prev);
	//int sum = diffp + diffd;
	//fprintf(fp,"ratep = %f, rated = %f\n", (double)diffp / sum, (double)diffd / sum);
	(*diff_sum) += ((*diff_prev) + diffp) / 2.0;
	(*diff_prev) = diffp;
	(*diff_sum) /= 2.0;
	float turnp = Kp * diffp + bias;
	float turni = Ki * diffi;
	float turnd = Kd * diffd;
	if (turnd > 10.0) {
		turnd = 10.0;
	}
	if (turnd < -10.0) {
		turnd = -10.0;
	}*/

	float turn = turnp + turni + turnd;
	fprintf(fp, "%d,%f,%f,%f,%f\n",time, turnp, turni, turnd, turn);
	return -turn;
}
