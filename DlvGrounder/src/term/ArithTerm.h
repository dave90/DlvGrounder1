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
 *  and the relative operator (+,-,*,M)
 *
 *  The calculation of the operation is based on the pattern strategy
 *
 */

class ArithTerm: public Term {
public:
	/// Take the TableTerm for the calculation
	ArithTerm(TermTable *t):Term(0),termTable(t){};
	///Set operator
	virtual void setOperator(Operator op){operators.push_back(op);};
	/// Calculate the operation based on the operator
	virtual double calculate();
	///Add term for the calculation
	virtual void addTerm(index_object termIndex){terms.push_back(termIndex);};
	virtual void popTerm(){terms.pop_back();};
	virtual string getNameToHash();
	virtual void print(TermTable* tb);
	/// Return the operator in string
	static string getNameOperator(Operator op);
private:
	// All the index of the terms to calculate the operation
	vector<index_object> terms;
	TermTable *termTable;

	// The operators
	vector<Operator> operators;
};

#endif /* ARITHTERM_H_ */

