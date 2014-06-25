/*
 * ArithTerm.cpp
 *
 *  Created on: 09/mar/2014
 *      Author: Davide
 */

#include "ArithTerm.h"

#include<boost/lexical_cast.hpp>

#include <sstream>

double ArithTerm::calculate() {
	TermTable *termTable=TermTable::getInstance();

	double result = termTable->getTerm(terms[0])->calculate();
	for (unsigned int i = 1; i < terms.size(); i++) {
		if (operators[i] == Operator::PLUS)
			result += termTable->getTerm(terms[i])->calculate();
		else if (operators[i] == Operator::MINUS)
			result -= termTable->getTerm(terms[i])->calculate();
		else if (operators[i] == Operator::DIV)
			result /= termTable->getTerm(terms[i])->calculate();
		else if (operators[i] == Operator::TIMES)
			result *= termTable->getTerm(terms[i])->calculate();
	}

	return result;
}

string ArithTerm::getNameToHash() {
	string name = "";
	for (unsigned int i = 0; i < terms.size() - 1; i++) {
		name += boost::lexical_cast<string>(terms[i]) + getNameOperator(operators[i]);
	}
	name += boost::lexical_cast<string>(terms[terms.size() - 1]);
	return name;
}

void ArithTerm::print(TermTable* tb) {
	for (unsigned int i = 0; i < terms.size() - 1; i++) {
		tb->getTerm(terms[i])->print(tb);
		cout  << getNameOperator(operators[i]);
	}
	tb->getTerm(terms[terms.size() - 1])->print(tb);
}

string ArithTerm::getNameOperator(Operator op) {
	if (op == Operator::PLUS)
		return "+";
	if (op == Operator::MINUS)
		return "-";
	if (op == Operator::TIMES)
		return "*";
	if (op == Operator::DIV)
		return "/";
	return "";
}
