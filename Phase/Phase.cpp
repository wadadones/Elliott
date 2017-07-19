// Phase.cpp

#include "Phase.hpp"

Phase::Phase(std::string name, Balancer* balancer)
{
	this->name = name;
	this->balancer = balancer;
}

std::string Phase::getName()
{
	return name;
}
