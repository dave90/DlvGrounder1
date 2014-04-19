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

	negativeTerm = false;
	id = "";

	negativeAtom = false;
	hashMinusAtom = false;

}

void StatementBuilder::printStats() {
	if (Config::getInstance()->isStatistic()) {
		cout<<endl;
		cout<<"Statistic"<<endl;
		cout << "Term table: " << Config::getInstance()->getTermTableType() << endl;
		cout << "Hash: " << Config::getInstance()->getHashType() << endl;
		cout << endl;
		cout << "Statement: " << statement << endl;
		cout << "Constraint: " << constraint << endl;
		cout << "Weak: " << weak << endl;
		cout << "Disjunction: " << disjunction << endl;
		cout << "Choice: " << choice << endl;
		cout << "Aggregate: " << aggregate << endl;
		cout << "Literal: " << literal << endl;
		cout << "Term: " << term << endl;
		cout << endl;

		cout << endl;
		Timer::getInstance()->print();
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

void StatementBuilder::addLiteral(string name) {
	ruleFactory.addPredicate(name);

	literal++;
}

void StatementBuilder::addClassicalAtom() {
	ruleFactory.addClassicalAtom(termsInAtom, hashMinusAtom, negativeAtom);

	termsInAtom.clear();
	hashMinusAtom = false;
	negativeAtom = false;
}

void StatementBuilder::setBinop(string binop) {
	ruleFactory.setBinop(binop);
}


void StatementBuilder::addBuiltinAtom() {
	ruleFactory.addBuiltinAtom(termsInAtom,getTermTable());

	termsInAtom.clear();
	hashMinusAtom = false;
	negativeAtom = false;
}

void StatementBuilder::setNegativeAtom() {
	negativeAtom = true;
}

void StatementBuilder::setStrongNegativeAtom() {
	hashMinusAtom = true;
}

void StatementBuilder::resetTerm() {
	id = "";
	negativeTerm = false;
}

void StatementBuilder::addVariable(string & name) {
	 long index = termsFactory.createVariable(name, negativeTerm);
	resetTerm();

	if (index != -1)
		termsInAtom.push_back(index);
	term++;
}

void StatementBuilder::addId(string & name) {

	 long index = termsFactory.createConstant(name, negativeTerm);
	resetTerm();

	if (index != -1)
		termsInAtom.push_back(index);
	term++;
}

void StatementBuilder::addNumber(int & name) {

	ostringstream convert;
	convert << name;
	string id = convert.str();
	 long index = termsFactory.createConstant(id, negativeTerm);
	resetTerm();

	if (index != -1)
		termsInAtom.push_back(index);
	term++;
}

void StatementBuilder::addTermFunction() {

	termsFactory.createFunction(id, negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addNameFunction(string & name) {
	id = name;
}

void StatementBuilder::endTermFunction() {

	 long index = termsFactory.endFunction();
	resetTerm();

	if (index != -1)
		termsInAtom.push_back(index);
}

void StatementBuilder::setNegativeTerm() {
	negativeTerm = true;
}

void StatementBuilder::addArithTerm() {

	termsFactory.addArithTerm();
	resetTerm();
}

void StatementBuilder::setArithOperator(string op){
	termsFactory.setArithOperator(op);
}

void StatementBuilder::endArithTerm(){
	 long index =termsFactory.endArithTerm();

	if (index != -1)
		termsInAtom.push_back(index);
}

void StatementBuilder::setRuleBody() {
	ruleFactory.setAtomInHead(false);
}

void StatementBuilder::addRule() {
	ruleFactory.addRule();
}
