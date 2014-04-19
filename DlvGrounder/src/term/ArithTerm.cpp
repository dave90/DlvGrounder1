/*
 * ArithTerm.cpp
 *
 *  Created on: 09/mar/2014
 *      Author: Davide
 */

#include "ArithTerm.h"

#include <sstream>

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

void ArithTerm::print(TermTable* tb) {

	for (unsigned int i = 0; i < terms.size(); i++){
		if(i%2!=0)cout<<getNameOperator(getOperator());
		tb->getTerm(terms[i])->print(tb);
	}
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
	return "";
}
