/*
 * FunctionTerm.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef FUNCTIONTERM_H_
#define FUNCTIONTERM_H_

/*
 *  Extends the Term class and represent a function term.
 */

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

	// All the index of the terms inside the function
	vector<unsigned long> terms;
};

#endif /* FUNCTIONTERM_H_ */
