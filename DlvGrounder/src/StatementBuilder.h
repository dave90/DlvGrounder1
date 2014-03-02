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
#include <string>

#include <boost/timer.hpp>

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
	void resetTerm();
	void addVariable(string & name);
	void addId(string & name);
	void addNumber(int & name);
	void addNameFunction(string & name);
	void addTermFunction();
	void endTermFunction();
	void setNegativeTerm();

private:
	int statement;
	int weak;
	int constraint;
	int disjunction;
	int choice;
	int aggregate;
	int literal;
	int term;

	bool negativeTerm;
	string id;

	TermFactory termsFactory;
};

#endif /* STATEMENTBUILDER_H_ */
