/*
 * VariableTerm.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef VARIABLETERM_H_
#define VARIABLETERM_H_

#include "Term.h"

using namespace std;

class VariableTerm: public Term {
public:
	VariableTerm();
	virtual string getName();
	virtual void setName(string name);
	virtual void addTerm(unsigned long termIndex);
	virtual vector<unsigned long> getTerms();
	virtual void print();
private:
	string variable;
};

#endif /* VARIABLETERM_H_ */
