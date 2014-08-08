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
		if (operators[i-1] == Operator::PLUS)
			result += termTable->getTerm(terms[i])->calculate();
		else if (operators[i-1] == Operator::MINUS)
			result -= termTable->getTerm(terms[i])->calculate();
		else if (operators[i-1] == Operator::DIV){
			double number=termTable->getTerm(terms[i])->calculate();
			//FIXME For not stop the program division by 0 is 0
			if(number != 0)
				result /= number;
			else
				result = 0;
		}else if (operators[i-1] == Operator::TIMES)
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

void ArithTerm::print() {
	TermTable *tb=TermTable::getInstance();
	for (unsigned int i = 0; i < terms.size() - 1; i++) {
		tb->getTerm(terms[i])->print();
		cout  << getNameOperator(operators[i]);
	}
	tb->getTerm(terms[terms.size() - 1])->print();
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

index_object ArithTerm::substitute(unordered_map<index_object, index_object>& substritutionTerm) {
	// Create a new arithmetic term replacing the term in a vector
	// Recursively call substitute for nested function
	// At the end add a new term in a table and return index

	Term *subTerm=new ArithTerm(operators);
	TermTable *termTable=TermTable::getInstance();
	for(index_object term:terms){
		index_object sub_index=termTable->getTerm(term)->substitute(substritutionTerm);
		subTerm->addTerm(sub_index);
	}
	return termTable->addTerm(subTerm);
}
