/*
 * StatementBuilder.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef STATEMENTBUILDER_H_
#define STATEMENTBUILDER_H_

/*
 *  StatementBuilder execute the action of the parser
 */

#include <string>

#include <boost/timer.hpp>

#include "TermFactory.h"

using namespace std;

class StatementBuilder {
public:
	StatementBuilder();
	//set table of terms type
	void setTableType(TermTableType type);
	// Print the statistics
	void printStats();
	void addStatement();
	void addWeak();
	void addConstraint();
	void addChoiche();
	void addDisjunction();
	void addAggregate();
	void addLiteral();
	// Reset all variable used for parser a term
	void resetTerm();
	// Add variable term
	void addVariable(string & name);
	// Add constant term
	void addId(string & name);
	// Add numeric costant term
	void addNumber(int & name);
	// Set the namo of the function
	void addNameFunction(string & name);
	// Add a function term in the queue of the parser function
	void addTermFunction();
	// Add a function in the table of the function
	void endTermFunction();
	// Set the current term a negative term
	void setNegativeTerm();

private:
	/*
	 *  Counter for the statistics
	 */
	int statement;
	int weak;
	int constraint;
	int disjunction;
	int choice;
	int aggregate;
	int literal;
	int term;

	/*
	 * Variable for parser a term
	 */
	bool negativeTerm;
	string id;

	TermFactory termsFactory;
};

#endif /* STATEMENTBUILDER_H_ */
