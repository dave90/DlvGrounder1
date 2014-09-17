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


/**
 *  Signle measurement of time containing time elapsed and label
 */
class TimerStamp {
public:
	/// Constructor with label and difference
	TimerStamp(string l):label(l),diff(0){};
	/// Print label and time elapsed
	void print(){cout<<label<<" : "<<diff<<endl;};
	//Set time elapsed
	void setTime(){diff=boost_timer.elapsed();};
	/// Start the timer
	void startTime(){boost_timer.restart();};
	/// Add timer
	void addTime(double time){diff+=time;};
	/// Return the label
	string getLabel(){return label;}
	/// Return the time elapsed
	double getTime(){return diff;}
private:
	/// Label of the single time measurement
	string label;
	/// Time elapsed
	double diff;

	/// library for time measurement
	boost::timer boost_timer;

};


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
	void start(const string& label);
	/// End the time
	void end();
	/// End the time and sum with a previus timer
	void endAndSum(const string &label);

	/// Print all the time
	void print();
	~Timer(){for(TimerStamp* timerStamp:stampsFinished)delete timerStamp;delete timer;};
private:
	///Private constructor
	Timer(){};

	static Timer* timer;
	/// All the times taken
	vector<TimerStamp*> stamps;

	/// library for time measurement
	boost::timer boost_timer;

	/// TimerStamp finished
	vector<TimerStamp*> stampsFinished;
};

#endif /* TIMER_H_ */

