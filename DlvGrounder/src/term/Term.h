/*
 * Term.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef TERM_H_
#define TERM_H_

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Term {
public:
	Term();
	long getIndex();
	void setIndex(long index);
	bool isNegative();
	void setNegative(bool n);
	bool operator==(const Term& t);
	virtual string getVariable()=0;
	virtual void setVariable(string name)=0;
	virtual string getConstant()=0;
	virtual void setCostant(string constant)=0;
	virtual void addTerm(int termIndex)=0;
	virtual vector<int> getTerms()=0;
	virtual void print()=0;
private:
	long index;
	bool negative;
};

#endif /* TERM_H_ */
