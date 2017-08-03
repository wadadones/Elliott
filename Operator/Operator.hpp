// Operator.h

#ifndef OPERATOR_H
#define OPERATOR_H

#include "../EV3/EV3.hpp"
#include "../Balancer/BalancerCPP.hpp"
#include "../Schedule/Schedule.hpp"
#include "../Phase/Phase.hpp"

class Operator {
private:
	EV3* ev3;
	Balancer* balancer;
	Schedule* schedule;
public:
	Operator(EV3* ev3, Balancer* balancer, Schedule* schedule);
	void run();
};

#endif
