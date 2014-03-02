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
	unsigned long getIndex();
	void setIndex(unsigned long index);
	bool isNegative();
	void setNegative(bool n);
	bool operator==(const Term& t);
	virtual string getName();
	virtual void setName(string name);
	virtual void addTerm(unsigned long termIndex);
	virtual vector<unsigned long> getTerms();
	virtual void print();
private:
	unsigned long index;
	bool negative;
};

#endif /* TERM_H_ */
