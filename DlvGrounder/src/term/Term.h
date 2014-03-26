/*
 * Term.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef TERM_H_
#define TERM_H_

/*
 *  Term is an abstract class that represent one general Term.
 *  All the term have one index that represent our id.
 *
 *  The structure of a Term is based on the Composite pattern
 */

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Term {
public:
	Term();
	Term(unsigned long i,bool b):index(i),negative(b){};
	//Get the index of the term
	unsigned long getIndex(){return index;};
	//Set the index of the term
	void setIndex(unsigned long index){	this->index=index;};
	// Return the negation of the term
	bool isNegative(){return negative;};
	//Set the negation of the term
	void setNegative(bool n){negative=n;};
	// Equal with term, based on the index of the terms
	bool operator==(const Term& t){	return t.index==index;};
	// Return the name of the term
	virtual string getName(){};
	//Set the name of the term
	virtual void setName(string name){};
	// Add the index of a term inside the function term, used only with the FunctionTerm
	virtual void addTerm(unsigned long termIndex){};
	// Return the terms in the function term, used only with the FunctionTerm
	virtual vector<unsigned long> getTerms(){};
	// Return the calculate for the arithmetic term
	virtual double calculate(){};
	// Return the string used to hash the term
	virtual string getNameToHash(){};
	//print with the cout the term
	virtual void print(){};
	virtual ~Term(){};
private:
	/*
	 * Index of a term
	 */
	unsigned long index;
	/*
	 *  True if the term is negated, else false
	 */
	bool negative;
};

#endif /* TERM_H_ */
