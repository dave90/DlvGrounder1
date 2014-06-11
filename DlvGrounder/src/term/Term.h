/*
 * Term.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef TERM_H_
#define TERM_H_


#include <string>
#include <vector>
#include <iostream>

#include "../table/IdsManager.h"

using namespace std;

/**
 *  Term is an abstract class that represent one general Term.
 *  All the term have one index that represent our id.
 *
 *  The structure of a Term is based on the Composite pattern
 */
class TermTable;

/// Operator for arith term
enum Operator {
	PLUS=0, MINUS, DIV, TIMES
};

class Term {
public:
	Term();
	/**
	 *  @param i the id
	 *  @param b if is negative
	 */
	Term(index_object i,bool b):index(i),negative(b){};
	Term(bool negative):index(0),negative(negative){};
	///Get the index of the term
	index_object getIndex(){return index;};
	/// Set the index of the term
	void setIndex(index_object index){	this->index=index;};
	///Return the negation of the term
	bool isNegative(){return negative;};
	///Set the negation of the term
	void setNegative(bool n){negative=n;};
	///Equal with term, based on the index of the terms
	bool operator==(const Term& t){	return t.index==index;};
	///Return the name of the term
	virtual string getName(){return "";};
	///Set the name of the term
	virtual void setName(string& name){};
	/// Return true if is anonymous variable
	virtual bool isAnonymous(){return false;};
	/// Return true if is variable
	virtual bool isVariable(){return false;};
	///Add the index of a term inside the function term, used only with the FunctionTerm
	virtual void addTerm(index_object termIndex){};
	///Remove last index of a term inside the function term, used only with the FunctionTerm
	virtual void popTerm(){};
	///Return the terms in the function term, used only with the FunctionTerm
	virtual vector<index_object> getTerms(){return vector<index_object>();};
	/// Set operator for Arith Term
	virtual void setOperator(Operator op){};
	/// Return the calculate for the arithmetic term
	virtual double calculate(){return 0;};
	/// Return the string used to hash the term
	virtual string getNameToHash(){return "";};
	/// Print with the cout the term
	virtual void print(TermTable *tb){cout<<IdsManager::getStringStrip(IdsManager::TERM_ID_MANAGER,getIndex());};
	virtual ~Term(){};
protected:
	/*
	 * Index of a term
	 */
		index_object index;
		/*
	 *  True if the term is negated, else false
	 */
	bool negative;
};

#endif /* TERM_H_ */
