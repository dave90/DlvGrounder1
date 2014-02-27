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
	virtual string getVariable();
	virtual void setVariable(string name);
	virtual string getConstant();
	virtual void setCostant(string constant);
	virtual void addTerm(int termIndex);
	virtual vector<int> getTerms();
	virtual void print();
private:
	string variable;
};

#endif /* VARIABLETERM_H_ */
