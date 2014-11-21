/*
 * AtomFactory.h
 *
 *  Created on: 27/mar/2014
 *      Author: Davide
 */

#ifndef ATOMFACTORY_H_
#define ATOMFACTORY_H_

#include "table/PredicateTable.h"
#include "table/Instance.h"
#include "table/TermTable.h"
#include "statement/Rule.h"
#include "ground/StatementDependency.h"

/**
 *  Manage the creation of the rules and literals
 */

class RuleFactory {
public:
	RuleFactory();
	/// Add predicate of the atom
	void addPredicate(string& name){lastPredicate=name;};
	/// Add classical atom in rule
	void addClassicalAtom(vector<Term*> &terms,bool hashMinus,bool negative);
	/// Set binop in builtin
	void setBinop(string& binop);
	/// Add builtin atom in rule
	void addBuiltinAtom(vector<Term*> &terms);
	/// Add Fact in Instances
	void addFact(Atom *fact);
	/// Set head false and put the next atoms in the body
	void setAtomInHead(bool b){head=b;};
	/// Add rule and if have head, set the predicates IDB
	void addRule();

	/// Return the InstanceTable
	InstancesTable* getInstancesTable() {return instancesTable;}

	/// Return PredicateTable
	PredicateTable* getPredicateTable() {return predicateTable;}

	/// Return the StatementDependency
	StatementDependency* getStatementDependency() {return statementDependency;}

	~RuleFactory(){delete currentRule;};


private:
	/**
	 *  Table of predicate
	 */
	PredicateTable* predicateTable;
	/**
	 *  Table of all the instances
	 */
	InstancesTable* instancesTable;

	/**
	 *  Dipendency of the atoms and rules
	 */
	StatementDependency* statementDependency;


	/*
	 *  Variable for parsing atom
	 */
	string lastPredicate;

	/*
	 *  Variable for parsing rule
	 */
	bool head;
	Rule *currentRule;
	Binop binop;

};

#endif /* ATOMFACTORY_H_ */
