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
#include "RuleFactory.h"

using namespace std;

class StatementBuilder {
public:
	StatementBuilder();

	// Print the statistics
	void printStats();
	void addStatement();
	void addWeak();
	void addConstraint();
	void addChoiche();
	void addDisjunction();
	void addAggregate();
	void setRuleBody();
	void addRule ();
	//Add predicate
	void addLiteral(string name);
	//Add atom in atomFactory
	void addClassicalAtom();
	// Set the current Atom a negative atom
	void setNegativeAtom();
	// Set the current Atom a strong negative atom
	void setStrongNegativeAtom();
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
	//Add Arithmetic term and pass the operator
	void addArithTerm(string &op);

	TermTable* getTermTable(){return termsFactory.getTermTable();};
	InstancesTable* getInstanceTable(){return ruleFactory.getInstancesTable();};
	StatementDependency* getStatementDependency(){return ruleFactory.getStatementDependency();};
	PredicateTable* getPredicateTable(){return ruleFactory.getPredicateTable();};

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

	/*
	 *  Variable for parsing atoms
	 */
	bool hashMinusAtom;
	bool negativeAtom;
	vector<unsigned long> termsInAtom;

	TermFactory termsFactory;
	RuleFactory ruleFactory;
};

#endif /* STATEMENTBUILDER_H_ */
