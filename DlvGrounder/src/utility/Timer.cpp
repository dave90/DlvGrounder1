/*
 * Timer.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Davide
 */

#include "Timer.h"
#include <iostream>

Timer* Timer::timer=0;

Timer* Timer::getInstance() {
	if(timer==0)
		timer = new Timer;
	return timer;
}

void Timer::start(const string& label) {
	TimerStamp *t=new TimerStamp(label);
	t->startTime();
	stamps.push_back(t);
}

void Timer::end() {
	TimerStamp *lastTimerStamp=stamps[stamps.size()-1];
	lastTimerStamp->setTime();
	stamps.pop_back();
	stampsFinished.push_back(lastTimerStamp);
}

void Timer::print() {
	for(unsigned int i=0;i<stampsFinished.size();i++)
		stampsFinished[i]->print();
}

void Timer::endAndSum(const string& label) {
	TimerStamp *lastTimerStamp=stamps[stamps.size()-1];
	lastTimerStamp->setTime();
	stamps.pop_back();
	bool find=false;
	for(auto timer:stampsFinished)
		if(timer->getLabel().compare(label.c_str())==0){
			timer->addTime(lastTimerStamp->getTime());
			find=true;
		}
	if(!find)
		stampsFinished.push_back(lastTimerStamp);
	else
		delete lastTimerStamp;
}
