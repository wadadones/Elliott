// Schedule.cpp

#include "Schedule.hpp"

void Schedule::pushPhase(Phase* phase)
{
	queue.push(phase);
}

Phase* Schedule::popPhase()
{
	Phase* phase = queue.front();
	queue.pop();
	return phase;
}

bool Schedule::isEmpty()
{
	return !queue.empty();
}
