/*
 * ArithTerm.cpp
 *
 *  Created on: 09/mar/2014
 *      Author: Davide
 */

#include "ArithTerm.h"
#include <sstream>

ArithTerm::ArithTerm(TermTable *t) {
	termTable=t;
}

double ArithTerm::calculate() {
	double result = termTable->getTerm(terms[0])->calculate();
	for (unsigned int i = 1; i < terms.size(); i++){
		if (getOperator() == Operator::PLUS)
			result += termTable->getTerm(terms[i])->calculate();
		else if (getOperator() == Operator::MINUS)
			result -=  termTable->getTerm(terms[i])->calculate();
		else if (getOperator() == Operator::DIV)
			result /=  termTable->getTerm(terms[i])->calculate();
		else if (getOperator() == Operator::TIMES)
			result *=  termTable->getTerm(terms[i])->calculate();
	}

	return result;
}

void ArithTerm::addTerm(unsigned long termIndex) {
	terms.push_back(termIndex);
}

ArithTerm::~ArithTerm() {
}

string ArithTerm::getNameToHash() {
	string name = getOperator()+"*";
	for (unsigned int i = 0; i < terms.size(); i++){
		ostringstream convert;
		convert << terms[i];
		string id=convert.str();
		name+=id+"*";
	}
	return name;
}

void ArithTerm::print() {

	cout<<"ArithTerm"<<getNameOperator(getOperator())<<" index "<<getIndex()<<" terms: ";
	for (unsigned int i = 0; i < terms.size(); i++)
		cout<<terms[i]<<" ";
	cout<<endl;
}

string ArithTerm::getNameOperator(Operator op) {
	if(op==Operator::PLUS)
		return "+";
	if(op==Operator::MINUS)
		return "-";
	if(op==Operator::TIMES)
		return "*";
	if(op==Operator::DIV)
		return "/";
}
