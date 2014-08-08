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
#include <unordered_map>

#include "../table/IdsManager.h"

using namespace std;

/// Operator of and arithmetic term
enum Operator {
	PLUS=0, MINUS=1, DIV=2, TIMES=3
};

typedef vector<pair<index_object,index_object>> vector_pair_index;

/**
 *  Term is an abstract class that represent one general Term.
 *  All the term have one index that represent our id.
 *
 *  The structure of a Term is based on the Composite pattern
 */
class Term {
public:
	/// Empty constructor
	Term();
	//Constructor only if is negative, set id in second time
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
	/// Return true if is an arithmetic term
	virtual bool isArithTerm(){return false;};
	///Add the index of a composite term
	virtual void addTerm(index_object termIndex){};
	///Remove last index of a term
	virtual void popTerm(){};
	///Return the terms from which it is composed
	virtual vector<index_object> getTerms(){return vector<index_object>();};
	/// Set operator for arithmetic Term
	virtual void setOperator(Operator op){};
	/// Calculate the value for arithmetic term
	virtual double calculate(){return 0;};
	/// Substitute the term with the given terms and return the index of substitute term
	/// @param substritutionTerm map of index_object. The first index is the ID of term to substitute and second the value
	virtual index_object substitute(unordered_map<index_object, index_object>& substritutionTerm){return getIndex();};
	/// Return the string used to hash the term
	virtual string getNameToHash(){return "";};
	/// Print with the cout the term
	virtual void print(){cout<<IdsManager::getStringStrip(IdsManager::TERM_ID_MANAGER,getIndex());};
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
