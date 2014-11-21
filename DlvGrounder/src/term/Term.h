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
#include <unordered_set>

#include "../table/Hashable.h"
#include "../table/IdsManager.h"

using namespace std;

/// Operator of and arithmetic term
enum Operator {
	PLUS=0, MINUS=1, DIV=2, TIMES=3
};

enum TermType{
	CONSTANT=0,VARIABLE,ARITH,FUNCTION,ANONYMOUS
};

class Term;

typedef unordered_map<Term*, Term*,IndexForTable<Term>,IndexForTable<Term>> map_term_term;
typedef unordered_set<Term*,IndexForTable<Term>,IndexForTable<Term>> set_term;

/**
 *  Term is an abstract class that represent one general Term.
 *  All the term have one index that represent our id.
 *
 *  The structure of a Term is based on the Composite pattern
 */
class Term: public Indexable,public Hashable {
public:
	/// Empty constructor
	Term():Indexable(),negative(0){};
	//Constructor only if is negative, set id in second time
	Term(bool negative):Indexable(),negative(negative){};
	//Constructor with negation and index
	Term(bool negative,index_object index):Indexable(index),negative(negative){};

	///Get the index of the term
	index_object getIndex() const {return index;};
	/// Set the index of the term
	void setIndex(index_object index){	this->index=index;};
	/// Return the string used to hash the term
	virtual size_t hash(){return 0;};


	///Return the negation of the term
	bool isNegative(){return negative;};
	///Set the negation of the term
	void setNegative(bool n){negative=n;};
	///Return the numeric term constant
	virtual int getConstantValue();
	///Return the name of the term
	virtual string getName()const{return "";};
	///Set the name of the term
	virtual void setName(string& name){};
	/// Return the size of the terms (Function and Arith)
	virtual unsigned getTermsSize()const{return 0;}
	///Return the term in terms
	virtual Term* getTerm(unsigned i)const{return nullptr;}
	///Return the size of the operators in arith term
	virtual unsigned int getSizeOperator()const{return 0;}
	///Return the operator with index i
	virtual Operator getOperator(int i)const{return Operator::PLUS;}

	///Return the type of term
	virtual TermType getType()const{return TermType::CONSTANT;};
	///Return true if contain a term of the given type
	virtual bool contain(TermType type){return false;};
	///Return true if is ground
	virtual bool isGround(){return false;}

	virtual bool operator==(const Term& term){
		if(getType()!=term.getType())return false;
		if(getName().compare(term.getName())!=0)return false;
		return true;
	}
	///Add the index of a composite term
	virtual void addTerm(Term* termIndex){};
	///Remove last index of a term
	virtual void popTerm(){};
	/// Set operator for arithmetic Term
	virtual void setOperator(Operator op){};
	/// Calculate the value for arithmetic term and return the ID of the result term
	virtual Term* calculate(){return this;};
	/// If the term is variable insert the variables in the vector
	virtual void getVariable(set_term& variables){void(0);};
	/// Substitute the term with the given terms and return the term of substitute term
	/// @param substritutionTerm map of index_object. The first index is the ID of term to substitute and second the value
	virtual Term* substitute(map_term_term& substritutionTerm){return nullptr;};
	/// Print with the cout the term
	virtual void print(){};
	/// Match a function with given id of term, compare the constant term and put in binds
	/// a value of the variable term present in termToMatch
	/// Return true if constant term are equal, else false
	virtual bool match(Term* termToMatch,map_term_term& varAssignment){return false;};
	virtual ~Term(){};
protected:
	/*
	 *  True if the term is negated, else false
	 */
	bool negative;
};

#endif /* TERM_H_ */
