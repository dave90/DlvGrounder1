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

#include "Term.h"

class ConstantTerm: public Term {
public:
	ConstantTerm();
	virtual string getName();
	virtual void setName(string name);
	virtual void addTerm(unsigned long termIndex);
	virtual vector<unsigned long> getTerms();
	virtual string getNameToHash();
	virtual void print();
private:
	string constant;
};

#endif /* CONSTANTTERM_H_ */
