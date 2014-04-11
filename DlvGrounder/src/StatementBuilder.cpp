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

	 negativeAtom=false;
	 hashMinusAtom=false;

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
	cout<<endl;

	cout<<endl;
	Timer::getInstance()->print();

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

void StatementBuilder::addLiteral(string name) {
	ruleFactory.addPredicate(name);

	literal++;
}

void StatementBuilder::addClassicalAtom() {
	ruleFactory.addClassicalAtom(termsInAtom,hashMinusAtom,negativeAtom);

	termsInAtom.clear();
	hashMinusAtom=false;
	negativeAtom=false;
}

void StatementBuilder::setNegativeAtom() {
	negativeAtom=true;
}

void StatementBuilder::setStrongNegativeAtom() {
	hashMinusAtom=true;
}

void StatementBuilder::resetTerm(){
	 id="";
	 negativeTerm=false;
}

void StatementBuilder::addVariable(string & name) {
	unsigned long index=termsFactory.createVariable(name,negativeTerm);
	resetTerm();

	if(index!=-1)termsInAtom.push_back(index);
	term++;
}

void StatementBuilder::addId(string & name) {

	unsigned long index=termsFactory.createConstant(name,negativeTerm);
	resetTerm();

	if(index!=-1)termsInAtom.push_back(index);
	term++;
}

void StatementBuilder::addNumber(int & name) {

	ostringstream convert;
	convert << name;
	string id=convert.str();
	unsigned long index=termsFactory.createConstant(id,negativeTerm);
	resetTerm();

	if(index!=-1)termsInAtom.push_back(index);
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

	unsigned long index=termsFactory.endFunction();
	resetTerm();

	if(index!=-1)termsInAtom.push_back(index);
}

void StatementBuilder::setNegativeTerm() {
	negativeTerm=true;
}

void StatementBuilder::addArithTerm(string &op) {

	unsigned long index=termsFactory.addArithTerm(op);
	if(index!=-1)termsInAtom.pop_back();
}

void StatementBuilder::setRuleBody() {
	ruleFactory.setAtomInHead(false);
}

void StatementBuilder::addRule() {
	ruleFactory.addRule();
}
