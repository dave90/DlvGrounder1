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
	ArithTerm(TermTable *t):termTable(t),op(Operator::PLUS){};
	/// Get the operator for the calculation
	virtual Operator getOperator(){return op;};
	///Set operator
	virtual void setOperator(Operator op){this->op=op;};
	/// Calculate the operation based on the operator
	virtual double calculate();
	///Add term for the calculation
	virtual void addTerm(unsigned long termIndex){terms.push_back(termIndex);};
	virtual string getNameToHash();
	virtual void print(TermTable* tb);
	/// Return the operator in string
	static string getNameOperator(Operator op);
private:
	// All the index of the terms to calculate the operation
	vector<unsigned long> terms;
	TermTable *termTable;

	// The operator
	Operator op;
};

#endif /* ARITHTERM_H_ */

