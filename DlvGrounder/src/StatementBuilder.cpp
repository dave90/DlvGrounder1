/*
 * StatementBuilder.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "StatementBuilder.h"
#include "utility/Timer.h"
#include "utility/Config.h"
#include "utility/IndexDefinition.h"


#include <cstring>
#include <iostream>
#include <sstream>

StatementBuilder::StatementBuilder() {
	statement = 0;
	weak = 0;
	constraint = 0;
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
		cout << "Index Type: " << Config::getInstance()->getIndexType() << endl;
		cout << endl;
		cout << "Statement: " << statement << endl;
		cout << "Constraint: " << constraint << endl;
		cout << "Weak: " << weak << endl;
		cout << "Choice: " << choice << endl;
		cout << "Aggregate: " << aggregate << endl;
		cout << "Literal encountered: " << literal << endl;
		cout << "Term encountered: " << term << endl;

		cout << endl;

		cout << endl;
		Timer::getInstance()->print();
	}

}


void StatementBuilder::addClassicalAtom() {
	vector<index_object> terms=move(termsFactory.getTermsInAtom());
	ruleFactory.addClassicalAtom(terms, hashMinusAtom, negativeAtom);

	termsFactory.clearTermsInAtom();
	hashMinusAtom = false;
	negativeAtom = false;
}



void StatementBuilder::addBuiltinAtom() {
	vector<index_object> terms=termsFactory.getTermsInAtom();
	ruleFactory.addBuiltinAtom(terms);

	termsFactory.clearTermsInAtom();
	hashMinusAtom = false;
	negativeAtom = false;
}

void StatementBuilder::addVariable(string & name) {
	 termsFactory.createVariable(name, negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addId(string & name) {

	termsFactory.createConstant(name, negativeTerm);
	resetTerm();


	term++;
}

void StatementBuilder::addNumber(unsigned int & name) {

	termsFactory.createConstant(name, negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addTermFunction() {

	termsFactory.createFunction(id, negativeTerm);
	resetTerm();

	term++;
}

