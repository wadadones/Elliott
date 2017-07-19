// app.cpp

#include "ev3api.h"
#include "app.hpp"
#include "app_sub.hpp"
#include "EV3/EV3.hpp"
#include "Balancer/balancer.h"
#include "Balancer/BalancerCPP.hpp"
#include "Operator/Operator.hpp"
#include "Schedule/Schedule.hpp"
#include "Phase/Phase.hpp"
#include "Phase/LineTracer/LineTracer.hpp"

//インスタンス作成
EV3 ev3;
Balancer balancer;
Schedule schedule;
Operator ope(&ev3, &balancer, &schedule);
LineTracer lt("LineTracer", &balancer);

//空のスケジュールにPhaseインスタンスをプッシュ
void createSchedule()
{
	schedule.pushPhase(&lt);
}

/* メインタスク */
void main_task(intptr_t unused)
{
	//スケジュールを作って
	createSchedule();
	//オペレータを走らせて
	ope.run();
	//おわり
	ext_tsk();
}

/*BlueTooth*タスク*/
void bt_task(intptr_t unused)
{
while(1)
{
	uint8_t c = fgetc(ev3.getBT()); /* 受信 */
	switch(c)
	{
	case '1':
		ev3.setBTcmd(1);
		break;
	default:
		break;
	}
	fputc(c, ev3.getBT()); /* エコーバック */
}
}
