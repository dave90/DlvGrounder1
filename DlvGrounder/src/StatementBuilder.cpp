/*
 * StatementBuilder.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "StatementBuilder.h"

StatementBuilder::StatementBuilder() {
	 statement = 0;
	 weak = 0;
	 constraint = 0;
	 disjunction = 0;
	 choice = 0;
	 aggregate = 0;
	 literal = 0;
	 term = 0;
	 variable="";
	 variable="";
	 id="";
	 number=0;
	 negativeTerm=false;
}

void StatementBuilder::printStats() {
	cout << "Statement: " << statement << endl;
	cout << "Constraint: " << constraint << endl;
	cout << "Weak: " << weak << endl;
	cout << "Disjunction: " << disjunction << endl;
	cout << "Choice: " << choice << endl;
	cout << "Aggregate: " << aggregate << endl;
	cout << "Literal: " << literal << endl;
	cout << "Term: " << term << endl;

	TermTable *tm=termsFactory.getMap();
	for(long i=0;i<tm->getSize();i++){
		tm->getTerm(i)->print();
	}
}

void StatementBuilder::addStatement() {
	statement++;
}

void StatementBuilder::addWeak() {
	weak++;
}

void StatementBuilder::addConstraint() {
}

void StatementBuilder::addChoiche() {
	choice++;
}

void StatementBuilder::addDisjunction() {
	disjunction++;
}

void StatementBuilder::addAggregate() {
	aggregate++;
}

void StatementBuilder::addLiteral() {
	resetLiteral();

	literal++;
}

void StatementBuilder::resetLiteral() {
	id="";
}

void StatementBuilder::addTerm() {
	if(variable.size()>0){
		termsFactory.createVariable(variable,negativeTerm);
	}else if(strcmp("+number",id.c_str())==0){
		ostringstream convert;
		convert << number;
		id=convert.str();
		termsFactory.createConstant(id,negativeTerm);
	}else if(id.size()>0){
		termsFactory.createConstant(id,negativeTerm);
	}


	resetTerm();

	term++;
}

void StatementBuilder::resetTerm(){
	 variable="";
	 id="";
	 number=0;
	 negativeTerm=false;
}

void StatementBuilder::addVariable(const char& c) {
	variable.push_back(c);
}

void StatementBuilder::addId(const char& c) {
	id.push_back(c);
}

void StatementBuilder::addNumber(const int n) {
	//Set id="+number" for remember that there is a number
	id="+number";
	number=n;
}

void StatementBuilder::addTermFunction() {
	termsFactory.createFunction(id,negativeTerm);
	resetTerm();
}

void StatementBuilder::endTermFunction() {
	termsFactory.endFunction();
	resetTerm();
}

void StatementBuilder::setNegativeTerm() {
	negativeTerm=true;
}
