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

enum Operator {
	PLUS, MINUS, DIV, TIMES
};

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
	ArithTerm(TermTable *t){	termTable=t;};
	/// Get the operator for the calculation
	virtual Operator getOperator()=0;
	/// Calculate the operation based on the operator
	virtual double calculate();
	///Add term for the calculation
	virtual void addTerm(unsigned long termIndex){terms.push_back(termIndex);};
	virtual string getNameToHash();
	virtual void print();
	/// Return the operator in string
	static string getNameOperator(Operator op);
private:
	// All the index of the terms to calculate the operation
	vector<unsigned long> terms;
	TermTable *termTable;
};

/**
 *  ArithTermP is the sum arithmetic term
 */
class ArithTermP: public ArithTerm {
public:
	ArithTermP(TermTable *t):ArithTerm(t){}

	virtual Operator getOperator(){
		return Operator::PLUS;
	}
};

/**
 *  ArithTermM is the subtraction arithmetic term
 */
class ArithTermM: public ArithTerm {
public:
	ArithTermM(TermTable *t):ArithTerm(t){}
	virtual Operator getOperator(){
		return Operator::MINUS;
	}
};

/**
 *  ArithTermD is the divisor arithmetic term
 */
class ArithTermD: public ArithTerm {
public:
	ArithTermD(TermTable *t):ArithTerm(t){}
	virtual Operator getOperator(){
		return Operator::DIV;
	}
};

/**
 *  ArithTermT is the multiplication arithmetic term
 */
class ArithTermT: public ArithTerm {
public:
	ArithTermT(TermTable *t):ArithTerm(t){}
	virtual Operator getOperator(){
		return Operator::TIMES;
	}
};


#endif /* ARITHTERM_H_ */

