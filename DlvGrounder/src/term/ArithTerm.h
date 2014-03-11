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

class ArithTerm: public Term {
public:
	//Take the TableTerm for the calculation
	ArithTerm(TermTable *t);
	// Get the operator for the calculation
	virtual Operator getOperator()=0;
	// Calculate the operation
	virtual double calculate();
	//Add term for the calculation
	virtual void addTerm(unsigned long termIndex);
	virtual string getNameToHash();
	virtual void print();
	// Return the operator in string
	static string getNameOperator(Operator op);
	virtual ~ArithTerm();
private:
	// All the index of the terms to calculate the operation
	vector<unsigned long> terms;
	TermTable *termTable;
};

class ArithTermP: public ArithTerm {
public:
	ArithTermP(TermTable *t):ArithTerm(t){}

	virtual Operator getOperator(){
		return Operator::PLUS;
	}
};

class ArithTermM: public ArithTerm {
public:
	ArithTermM(TermTable *t):ArithTerm(t){}
	virtual Operator getOperator(){
		return Operator::MINUS;
	}
};

class ArithTermD: public ArithTerm {
public:
	ArithTermD(TermTable *t):ArithTerm(t){}
	virtual Operator getOperator(){
		return Operator::DIV;
	}
};

class ArithTermT: public ArithTerm {
public:
	ArithTermT(TermTable *t):ArithTerm(t){}
	virtual Operator getOperator(){
		return Operator::TIMES;
	}
};


#endif /* ARITHTERM_H_ */

