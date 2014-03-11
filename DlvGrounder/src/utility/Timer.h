/*
 * Timer.h
 *
 *  Created on: 08/mar/2014
 *      Author: Davide
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <string>
#include <vector>

#include <boost/timer.hpp>

using namespace std;

class TimerStamp;

class Timer {
public:
	static Timer* getInstance();
	void start(string label);
	void end();
	void print();
	~Timer();
private:
	Timer();
	static Timer* timer;
	vector<TimerStamp> stamps;
	string currentLabel;
	boost::timer boost_timer;
};

class TimerStamp {
public:
	TimerStamp(string l,double d);
	void print();
private:
	string label;
	double diff;
};
#endif /* TIMER_H_ */

