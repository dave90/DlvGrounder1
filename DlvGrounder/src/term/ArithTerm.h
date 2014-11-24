/*
 * ArithTerm.h
 *
 *  Created on: 09/mar/2014
 *      Author: Davide
 */

#ifndef ARITHTERM_H_
#define ARITHTERM_H_

#include <vector>

#include "Term.h"
#include "../table/TermTable.h"


/**
 *  ArithTerm is an arithmetic with the list of a terms
 *  and the relative operator (+,-,*,/)
 *
 *  Contains vector of id terms and vectors of relative operators
 *
 */

class ArithTerm: public Term {
public:
	ArithTerm():Term(){};
	ArithTerm(vector<Operator>& operators):operators(operators){};
	ArithTerm(bool negative,vector<Operator>& operators,vector<Term*>& terms):Term(negative),operators(operators),terms(terms){};

	///Add an operator in the vector
	virtual void setOperator(Operator op){operators.push_back(op);};
	virtual void addTerm(Term* term){terms.push_back(term);};

	virtual unsigned getTermsSize()const{return terms.size();}
	virtual Term* getTerm(unsigned i)const{return terms[i];}

	///Return the size of the operators in arith term
	virtual unsigned int getSizeOperator()const{return operators.size();}
	virtual Operator getOperator(int i) const {return operators[i];}

	/// Calculate the value based on the operators and terms
	virtual Term* calculate();
	virtual void popTerm(){terms.pop_back();};
	virtual Term* substitute(map_term_term& substritutionTerm);

	virtual TermType getType()const{return TermType::ARITH;};
	virtual bool contain(TermType type){
		if(type==TermType::ARITH) return true;
		for(auto term:terms)
			if(term->contain(type))
				return true;
		return false;
	}
	virtual bool isGround(){
		for(auto term:terms)
			if(!term->isGround())
				return false;
		return true;
	}

	virtual bool operator==(const Term& term);

	/// Return the string composed by the concatenation of terms and operators
	virtual size_t hash();
	virtual void print();
	/// Return the string of enum operator
	static string getNameOperator(Operator op);



private:
	/*
	 * The operators
	 */
	vector<Operator> operators;
	/**
	 *  All the index of the terms to calculate the operation
	 */
	vector<Term*> terms;
};

#endif /* ARITHTERM_H_ */

