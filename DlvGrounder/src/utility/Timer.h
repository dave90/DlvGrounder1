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
#include <iostream>

#include <boost/timer.hpp>

using namespace std;

class TimerStamp;

/**
 *  Utility class for manage the measurement of the times
 *  For the measurement of time is used the boost timer library
 *
 *  The class is a singleton
 */
class Timer {
public:
	/// Return the instance of the class
	static Timer* getInstance();
	/// Start the timer and assign the specified label
	void start(string label);
	/// End the time
	void end();
	/// Print all the time
	void print();
	~Timer(){delete timer;};
private:
	///Private constructor
	Timer(){};

	static Timer* timer;
	/// All the times taken
	vector<TimerStamp> stamps;
	/// Current label of the timer
	string currentLabel;

	/// library for time measurement
	boost::timer boost_timer;
};

/**
 *  Signle measurement of time containing time elapsed and label
 */
class TimerStamp {
public:
	/// Constructor with label and difference
	TimerStamp(string l,double d):label(l),diff(d){};
	/// Print label and time elapsed
	void print(){cout<<label<<" : "<<diff<<endl;};
private:
	/// Label of the single time measurement
	string label;
	/// Time elapsed
	double diff;
};
#endif /* TIMER_H_ */

