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

/*
 *  Represent a variable term
 */
class VariableTerm: public Term {
public:
	VariableTerm(){};
	VariableTerm(string v,unsigned long index):variable(v){setIndex(index);};
	virtual string getName(){return variable;};
	virtual void setName(string name){	variable=name;};
	virtual string getNameToHash(){	return variable;};
private:
	string variable;
};

#endif /* VARIABLETERM_H_ */
