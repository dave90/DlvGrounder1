/*
 * ArithTerm.cpp
 *
 *  Created on: 09/mar/2014
 *      Author: Davide
 */

#include "ArithTerm.h"
#include "../term/ConstantTerm.h"

#include<boost/lexical_cast.hpp>
#include "../hash/HashString.h"
#include "../hash/HashVecInt.h"

Term* ArithTerm::calculate() {
	unsigned int result = terms[0]->getConstantValue();
	for (unsigned int i = 1; i < terms.size(); i++) {
		unsigned int value=terms[i]->getConstantValue();
		if (operators[i-1] == Operator::PLUS)
			result += value;
		else if (operators[i-1] == Operator::MINUS)
			result -= value;
		else if (operators[i-1] == Operator::DIV){
			//FIXME For not stop the program division by 0 is 0
			if(value != 0)
				result /= value;
			else
				result = 0;
		}else if (operators[i-1] == Operator::TIMES)
			result *= value;
	}

	Term *constantTerm=new NumericConstantTerm(negative,result);
	TermTable::getInstance()->addTerm(constantTerm);
	return constantTerm;
}

size_t ArithTerm::hash() {
	vector<size_t> hashVec(terms.size()+operators.size());
	hashVec[0]=terms[0]->getIndex();
	for(unsigned int i=0;i<operators.size();i++){
		hashVec[i*2+1]=operators[i];
		hashVec[i*2+2]=terms[i+1]->getIndex();
	}
	return HashVecInt::getHashVecIntFromConfig()->computeHashSize_T(hashVec);
}

void ArithTerm::print() {
	for (unsigned int i = 0; i < terms.size() - 1; i++) {
		terms[i]->print();
		cout  << getNameOperator(operators[i]);
	}
	terms[terms.size() - 1]->print();
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

 bool ArithTerm::operator==(const Term& term){
	if(getType()!=term.getType())return false;
	if(operators.size()!=term.getSizeOperator())return false;
	if(terms.size()!=term.getTermsSize())return false;
	for(unsigned int i=0;i<operators.size();i++)
		if(operators[i]!=term.getOperator(i))
			return false;
	for(unsigned int i=0;i<terms.size();i++)
		if(terms[i]->getIndex()!=term.getTerm(i)->getIndex())
			return false;

	return true;
}

Term* ArithTerm::substitute(map_term_term& substritutionTerm) {
	// Create a new arithmetic term replacing the term in a vector
	// Recursively call substitute for nested function
	// At the end add a new term in a table and return index

	TermTable *termTable=TermTable::getInstance();
	vector<Term*> subTerms(terms.size());
	for(unsigned int i=0;i<terms.size();i++){
		Term* sub_term=terms[i]->substitute(substritutionTerm);
		subTerms[i]=sub_term;
	}

	Term *newTerm=new ArithTerm(negative,operators,subTerms);
	termTable->addTerm(newTerm);
	return  newTerm;
}
