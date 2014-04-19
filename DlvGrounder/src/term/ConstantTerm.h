/*
 * ConstantTerm.h
 *
 *  Created on: 27/feb/2014
 *      Author: Davide
 */

#ifndef CONSTANTTERM_H_
#define CONSTANTTERM_H_

#include <stdlib.h>

#include "Term.h"



/*
 *  Represent a constant term (string,number...)
 */
class ConstantTerm: public Term {
public:
	ConstantTerm(){};
	ConstantTerm(string v,unsigned long index):constant(v){setIndex(index);};
	virtual string getName(){return constant;};
	virtual void setName(string name){constant=name;};
	virtual string getNameToHash(){return constant;};
	virtual double calculate(){	return atof(constant.c_str());};
private:
	string constant;
};

#endif /* CONSTANTTERM_H_ */
