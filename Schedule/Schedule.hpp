// Schedule.h

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "../Phase/Phase.hpp"
#include <queue>

class Schedule {
private: 
	std::queue<Phase*> queue;
public:
	void pushPhase(Phase* phase);
	Phase* popPhase();
	bool isEmpty();
};

#endif
