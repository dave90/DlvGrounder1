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
	ArithTerm():Term(0){};
	ArithTerm(vector<Operator> operators):operators(operators){};
	///Ad an operator in the vector
	virtual void setOperator(Operator op){operators.push_back(op);};
	/// Calculate the value based on the operators and terms
	virtual double calculate();
	virtual void addTerm(index_object termIndex){terms.push_back(termIndex);};
	virtual void popTerm(){terms.pop_back();};
	virtual index_object substitute(unordered_map<index_object, index_object>& substritutionTerm);
	virtual bool isArithTerm(){return true;};

	/// If one term are variable then return false, else true
	virtual bool isVariable(){
		for(index_object term:terms)
			if(TermTable::getInstance()->getTerm(term)->isVariable())return false;
		return true;
	}

	virtual bool isConstant(){
		for(index_object term:terms)
			if(!TermTable::getInstance()->getTerm(term)->isConstant())return false;
		return true;
	};



	/// Return the string composed by the concatenation of terms and operators
	virtual string getNameToHash();
	virtual void print();
	/// Return the string of enum operator
	static string getNameOperator(Operator op);
private:
	/**
	 *  All the index of the terms to calculate the operation
	 */
	vector<index_object> terms;
	/*
	 * The operators
	 */
	vector<Operator> operators;
};

#endif /* ARITHTERM_H_ */

