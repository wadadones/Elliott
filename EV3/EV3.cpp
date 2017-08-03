// EV3.cpp

#include "EV3.hpp"

/*�Z���T�[�̐ڑ����`���܂�*/
static const sensor_port_t
	touch_sensor    = EV3_PORT_1,
	sonar_sensor    = EV3_PORT_2,
	color_sensor    = EV3_PORT_3,
	gyro_sensor     = EV3_PORT_4;

/*���[�^�̐ڑ����`���܂�*/
static const motor_port_t
	left_motor      = EV3_PORT_C,
	right_motor     = EV3_PORT_B,
	tail_motor      = EV3_PORT_A;

/* LCD�t�H���g�T�C�Y */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

/* ���S��~���̊p�x[�x] */
#define TAIL_ANGLE_STAND_UP  90//93
/* ���S��~�p���[�^������W�� */
#define P_GAIN             2.5F
/* ���S��~�p���[�^����PWM��΍ő�l */
#define PWM_ABS_MAX          60

/* �g�[���̍��� */
#define TONE_F              500
/* �g�[���̒��� */
#define TONE_T             1000

/* Bluetooth�t�@�C���n���h�� */
static FILE     *bt = NULL;
/* Bluetooth�R�}���h 1:�����[�g�X�^�[�g */
static int      bt_cmd = 0;

void EV3::setUpEV3()
{
	/* �Z���T�[���̓|�[�g�̐ݒ� */
	ev3_sensor_config(sonar_sensor, ULTRASONIC_SENSOR);
	ev3_sensor_config(color_sensor, COLOR_SENSOR);
	ev3_color_sensor_get_reflect(color_sensor); /* ���˗����[�h */
	ev3_sensor_config(touch_sensor, TOUCH_SENSOR);
	ev3_sensor_config(gyro_sensor, GYRO_SENSOR);

	/* ���[�^�[�o�̓|�[�g�̐ݒ� */
	ev3_motor_config(left_motor, LARGE_MOTOR);
	ev3_motor_config(right_motor, LARGE_MOTOR);
	ev3_motor_config(tail_motor, LARGE_MOTOR);
	ev3_motor_reset_counts(tail_motor);

	/* LCD��ʕ\�� */
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	ev3_lcd_draw_string("EV3way-ET sample_c4", 0, CALIB_FONT_HEIGHT*1);

	/* Open Bluetooth file */
	bt = ev3_serial_open_file(EV3_SERIAL_BT);
	assert(bt != NULL);

	/* Bluetooth�ʐM�^�X�N�̋N�� */
	act_tsk(BT_TASK);

	/* �����������ʒm */
	ev3_led_set_color(LED_ORANGE);
}

void EV3::waitForStart()
{
	/* �X�^�[�g�ҋ@ */
	while(1){
		ev3_tail_set_angle(TAIL_ANGLE_STAND_UP); /* ���S��~�p�p�x�ɐ��� */

		if (bt_cmd == 1){
			break; /* �����[�g�X�^�[�g */
		}

		if (ev3_touch_sensor_is_pressed(touch_sensor) == 1){
			break; /* �^�b�`�Z���T�������ꂽ */
		}

		tslp_tsk(10); /* 10msec�E�F�C�g */
	}

	/* �X�^�[�g�ʒm */
	ev3_led_set_color(LED_GREEN);
}

void EV3::resetSensorValue()
{
	/* ���s���[�^�[�G���R�[�_�[���Z�b�g */
	ev3_motor_reset_counts(left_motor);
	ev3_motor_reset_counts(right_motor);

	/* �W���C���Z���T�[���Z�b�g */
	ev3_gyro_sensor_reset(gyro_sensor);
}

void EV3::playTone()
{
	ev3_speaker_play_tone(TONE_F, TONE_T);
}

void EV3::getSensorValue(int32_t* motor_ang_l, int32_t* motor_ang_r, int* color, int* gyro, int* sonic, int *volt)
{
	*motor_ang_l = ev3_motor_get_counts(left_motor);
	*motor_ang_r = ev3_motor_get_counts(right_motor);
	*color = ev3_color_sensor_get_reflect(color_sensor);
	*gyro = ev3_gyro_sensor_get_rate(gyro_sensor);
	*sonic = ev3_ultrasonic_sensor_get_distance(sonar_sensor);
	*volt = ev3_battery_voltage_mV();
}

void EV3::setMotorValue(signed char pwm_L, signed char pwm_R, signed int ang_T)
{
	if (pwm_L == 0){
		ev3_motor_stop(left_motor, true);
	}else{
		ev3_motor_set_power(left_motor, (int)pwm_L);
	}
	if (pwm_R == 0){
		ev3_motor_stop(right_motor, true);
	}else{
		ev3_motor_set_power(right_motor, (int)pwm_R);
	}
	ev3_tail_set_angle(ang_T);
}

void EV3::sleep(int sleep_sec)
{
	tslp_tsk(sleep_sec);
}

bool EV3::buttonIsPressed()
{
	return ev3_button_is_pressed(BACK_BUTTON);
}

void EV3::shutDownEV3()
{
	ev3_motor_stop(left_motor, false);
	ev3_motor_stop(right_motor, false);

	ter_tsk(BT_TASK);
	fclose(bt);
}

void EV3::ev3_tail_set_angle(signed int angle)
{
	/* ��ᐧ�� */
	float pwm = (float)(angle - ev3_motor_get_counts(tail_motor))*P_GAIN;
	/* PWM�o�͖O�a���� */
	if (pwm > PWM_ABS_MAX){
		pwm = PWM_ABS_MAX;
	}
	else if (pwm < -PWM_ABS_MAX){
		pwm = -PWM_ABS_MAX;
	}

	if (pwm == 0){
		ev3_motor_stop(tail_motor, true);
	}
	else{
		ev3_motor_set_power(tail_motor, (signed char)pwm);
	}
}

FILE* EV3::getBT()
{
	return bt;
}

void EV3::setBTcmd(int cmd)
{
	bt_cmd = cmd;
}
