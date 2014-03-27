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
		timer =new Timer;
	return timer;
}

void Timer::start(string label) {
	currentLabel=label;
	boost_timer.restart();
}

void Timer::end() {
	TimerStamp ts(currentLabel,boost_timer.elapsed());
	stamps.push_back(ts);
}

void Timer::print() {
	for(unsigned int i=0;i<stamps.size();i++)
		stamps[i].print();
}

Timer::~Timer() {
	delete timer;
}

Timer::Timer() {

}

TimerStamp::TimerStamp(string l,double d) {
	label=l;
	diff=d;
}


void TimerStamp::print() {
	cout<<label<<" : "<<diff<<endl;
}
