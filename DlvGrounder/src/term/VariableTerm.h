/*
 * VariableTerm.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef VARIABLETERM_H_
#define VARIABLETERM_H_

/*
 *  Extends the Term class and represent a variable term
 */

#include "Term.h"

using namespace std;

class VariableTerm: public Term {
public:
	VariableTerm();
	VariableTerm(string v,unsigned long index):variable(v){setIndex(index);};
	virtual string getName(){return variable;};
	virtual void setName(string name){	variable=name;};
	virtual void addTerm(unsigned long termIndex){};
	virtual vector<unsigned long> getTerms(){};
	virtual string getNameToHash(){	return variable;};
	virtual void print();
private:
	string variable;
};

#endif /* VARIABLETERM_H_ */
