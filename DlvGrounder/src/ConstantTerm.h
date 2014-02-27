/*
 * ConstantTerm.h
 *
 *  Created on: 27/feb/2014
 *      Author: Davide
 */

#ifndef CONSTANTTERM_H_
#define CONSTANTTERM_H_

#include "Term.h"

class ConstantTerm: public Term {
public:
	ConstantTerm();
	virtual string getVariable();
	virtual void setVariable(string name);
	virtual string getConstant();
	virtual void setCostant(string constant);
	virtual void addTerm(int termIndex);
	virtual vector<int> getTerms();
	virtual void print();
private:
	string constant;
};

#endif /* CONSTANTTERM_H_ */
