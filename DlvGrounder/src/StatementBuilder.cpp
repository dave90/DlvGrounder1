/*
 * StatementBuilder.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */



#include "StatementBuilder.h"
#include "utility/Timer.h"
#include "utility/Config.h"


#include <cstring>
#include <iostream>
#include <sstream>

StatementBuilder::StatementBuilder() {
	 statement = 0;
	 weak = 0;
	 constraint = 0;
	 disjunction = 0;
	 choice = 0;
	 aggregate = 0;
	 literal = 0;
	 term = 0;

	 negativeTerm=false;
	 id="";

}


void StatementBuilder::printStats() {
	cout<<"Term table: "<<Config::getInstance()->getTermTableType()<<endl;
	cout<<"Hash: "<<Config::getInstance()->getHashType()<<endl;
	cout<<endl;
	cout << "Statement: " << statement << endl;
	cout << "Constraint: " << constraint << endl;
	cout << "Weak: " << weak << endl;
	cout << "Disjunction: " << disjunction << endl;
	cout << "Choice: " << choice << endl;
	cout << "Aggregate: " << aggregate << endl;
	cout << "Literal: " << literal << endl;
	cout << "Term: " << term << endl;

	Timer::getInstance()->start("Time read");
	TermTable *tm=termsFactory.getMap();
	cout<<"Size TermTable "<<tm->getSize()<<endl;
	for(long i=0;i<tm->getSize();i++){
		tm->getTerm(i);
	}
	Timer::getInstance()->end();

	Timer::getInstance()->print();

	cout<<endl;
	cout<<"Collision term Id Manager: "<<tm->getCollision()<<endl;

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

	literal++;
}



void StatementBuilder::resetTerm(){
	 id="";
	 negativeTerm=false;
}

void StatementBuilder::addVariable(string & name) {
	termsFactory.createVariable(name,negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addId(string & name) {
	termsFactory.createConstant(name,negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addNumber(int & name) {
	ostringstream convert;
	convert << name;
	string id=convert.str();
	termsFactory.createConstant(id,negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addTermFunction() {
	termsFactory.createFunction(id,negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addNameFunction(string & name) {
	id=name;
}

void StatementBuilder::endTermFunction() {
	termsFactory.endFunction();
	resetTerm();
}

void StatementBuilder::setNegativeTerm() {
	negativeTerm=true;
}

void StatementBuilder::addArithTerm(string &op) {
	termsFactory.addArithTerm(op);
}
