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
	virtual string getVariable();
	virtual void setVariable(string name);
	virtual string getConstant();
	virtual void setCostant(string constant);
	virtual void addTerm(int termIndex);
	virtual vector<int> getTerms();
	virtual void print();
private:
	string name;
	vector<int> terms;
};

#endif /* FUNCTIONTERM_H_ */
