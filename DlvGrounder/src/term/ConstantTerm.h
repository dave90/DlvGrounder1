/*
 * ConstantTerm.h
 *
 *  Created on: 27/feb/2014
 *      Author: Davide
 */

#ifndef CONSTANTTERM_H_
#define CONSTANTTERM_H_

/*
 *  Extends the Term class and represent a costant term
 */
#include <stdlib.h>

#include "Term.h"



class ConstantTerm: public Term {
public:
	ConstantTerm(){};
	ConstantTerm(string v,unsigned long index):constant(v){setIndex(index);};
	virtual string getName(){return constant;};
	virtual void setName(string name){constant=name;};
	virtual void addTerm(unsigned long termIndex){};
	virtual vector<unsigned long> getTerms(){};
	virtual string getNameToHash(){return constant;};
	virtual double calculate(){	return atof(constant.c_str());};
	virtual void print();
private:
	string constant;
};

#endif /* CONSTANTTERM_H_ */
