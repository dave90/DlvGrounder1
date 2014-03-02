/*
 * FunctionTerm.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef FUNCTIONTERM_H_
#define FUNCTIONTERM_H_

#include "Term.h"

using namespace std;

class FunctionTerm: public Term {
public:
	FunctionTerm();
	virtual string getName();
	virtual void setName(string name);
	virtual void addTerm(unsigned long termIndex);
	virtual vector<unsigned long> getTerms();
	virtual void print();
private:
	string name;
	vector<unsigned long> terms;
};

#endif /* FUNCTIONTERM_H_ */
