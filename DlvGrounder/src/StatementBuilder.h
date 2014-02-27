/*
 * StatementBuilder.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef STATEMENTBUILDER_H_
#define STATEMENTBUILDER_H_

#include <cstring>
#include <iostream>
#include <sstream>


#include "TermFactory.h"

using namespace std;

class StatementBuilder {
public:
	StatementBuilder();
	void printStats();
	void addStatement();
	void addWeak();
	void addConstraint();
	void addChoiche();
	void addDisjunction();
	void addAggregate();
	void addLiteral();
	void resetLiteral();
	void addTerm();
	void resetTerm();
	void addVariable(const char &c);
	void addId(const char &c);
	void addNumber(const int n);
	void addTermFunction();
	void endTermFunction();

private:
	int statement;
	int weak;
	int constraint;
	int disjunction;
	int choice;
	int aggregate;
	int literal;
	int term;
	string variable;
	string id;
	int number;

	TermFactory termsFactory;
};

#endif /* STATEMENTBUILDER_H_ */
