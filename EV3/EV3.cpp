// EV3.cpp

#include "EV3.hpp"

/*センサーの接続を定義します*/
static const sensor_port_t
	touch_sensor    = EV3_PORT_1,
	sonar_sensor    = EV3_PORT_2,
	color_sensor    = EV3_PORT_3,
	gyro_sensor     = EV3_PORT_4;

/*モータの接続を定義します*/
static const motor_port_t
	left_motor      = EV3_PORT_C,
	right_motor     = EV3_PORT_B,
	tail_motor      = EV3_PORT_A;

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

/* 完全停止時の角度[度] */
#define TAIL_ANGLE_STAND_UP  93
/* 完全停止用モータ制御比例係数 */
#define P_GAIN             2.5F
/* 完全停止用モータ制御PWM絶対最大値 */
#define PWM_ABS_MAX          60

/* トーンの高さ */
#define TONE_F              500
/* トーンの長さ */
#define TONE_T             1000

/* Bluetoothファイルハンドル */
static FILE     *bt = NULL;
/* Bluetoothコマンド 1:リモートスタート */
static int      bt_cmd = 0;

void EV3::setUpEV3()
{
	/* センサー入力ポートの設定 */
	ev3_sensor_config(sonar_sensor, ULTRASONIC_SENSOR);
	ev3_sensor_config(color_sensor, COLOR_SENSOR);
	ev3_color_sensor_get_reflect(color_sensor); /* 反射率モード */
	ev3_sensor_config(touch_sensor, TOUCH_SENSOR);
	ev3_sensor_config(gyro_sensor, GYRO_SENSOR);

	/* モーター出力ポートの設定 */
	ev3_motor_config(left_motor, LARGE_MOTOR);
	ev3_motor_config(right_motor, LARGE_MOTOR);
	ev3_motor_config(tail_motor, LARGE_MOTOR);
	ev3_motor_reset_counts(tail_motor);

	/* LCD画面表示 */
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	ev3_lcd_draw_string("EV3way-ET sample_c4", 0, CALIB_FONT_HEIGHT*1);

	/* Open Bluetooth file */
	bt = ev3_serial_open_file(EV3_SERIAL_BT);
	assert(bt != NULL);

	/* Bluetooth通信タスクの起動 */
	act_tsk(BT_TASK);

	/* 初期化完了通知 */
	ev3_led_set_color(LED_ORANGE);
}

void EV3::waitForStart()
{
	/* スタート待機 */
	while(1){
		ev3_tail_set_angle(TAIL_ANGLE_STAND_UP); /* 完全停止用角度に制御 */

		if (bt_cmd == 1){
			break; /* リモートスタート */
		}

		if (ev3_touch_sensor_is_pressed(touch_sensor) == 1){
			break; /* タッチセンサが押された */
		}

		tslp_tsk(10); /* 10msecウェイト */
	}

	/* スタート通知 */
	ev3_led_set_color(LED_GREEN);
}

void EV3::resetSensorValue()
{
	/* 走行モーターエンコーダーリセット */
	ev3_motor_reset_counts(left_motor);
	ev3_motor_reset_counts(right_motor);

	/* ジャイロセンサーリセット */
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
	/* 比例制御 */
	float pwm = (float)(angle - ev3_motor_get_counts(tail_motor))*P_GAIN;
	/* PWM出力飽和処理 */
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
