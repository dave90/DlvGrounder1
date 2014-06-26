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
	FunctionTerm():Term(0),name(0){};
	FunctionTerm(string& name,bool negative):Term(negative),name(name){};
	FunctionTerm(string v,index_object index):name(v){setIndex(index);};
	virtual string getName(){return name;};
	virtual void setName(string& name){this->name=name;};
	virtual void addTerm(index_object termIndex){terms.push_back(termIndex);};
	virtual void popTerm(){terms.pop_back();};
	virtual vector<index_object> getTerms(){return terms;};
	virtual string getNameToHash();
	virtual void print();
private:
	string name;

	// All the index of the terms inside the function
	vector<index_object> terms;
};

#endif /* FUNCTIONTERM_H_ */
