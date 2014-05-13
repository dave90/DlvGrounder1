/*
 * FunctionTerm.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef FUNCTIONTERM_H_
#define FUNCTIONTERM_H_


#include "Term.h"

#include "../table/TermTable.h"


using namespace std;

/*
 *  Represent a functional term.
 */
class FunctionTerm: public Term {
public:
	FunctionTerm();
	FunctionTerm(string v,unsigned long index):name(v){setIndex(index);};
	virtual string getName(){return name;};
	virtual void setName(string& name){this->name=name;};
	virtual void addTerm(unsigned long termIndex){terms.push_back(termIndex);};
	virtual void popTerm(){terms.pop_back();};
	virtual vector<unsigned long> getTerms(){return terms;};
	virtual string getNameToHash();
	virtual void print(TermTable *tb);
private:
	string name;

	// All the index of the terms inside the function
	vector<unsigned long> terms;
};

#endif /* FUNCTIONTERM_H_ */
