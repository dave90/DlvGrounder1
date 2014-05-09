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

class RuleFactory {
public:
	RuleFactory();
	/// Method for creation of Atom
	void addPredicate(string name);
	/// Add classical atom in rule
	void addClassicalAtom(vector<unsigned long> &terms,bool hashMinus,bool negative);
	/// Set binop in builtin
	void setBinop(string binop);
	/// Add builtin atom in rule
	void addBuiltinAtom(vector<unsigned long> &terms,TermTable*tt);
	/// Add Fact in Instances
	void addFact(Atom *fact);
	/// Set head false and put the next atoms in the body
	void setAtomInHead(bool b){head=b;};
	/// Add rule and if have head set the predicates IDB
	void addRule();

	 InstancesTable* getInstancesTable() {return instancesTable;}

	 PredicateTable* getPredicateTable() {return predicateTable;}

	 StatementDependency* getStatementDependency() {return st;}

	~RuleFactory(){};


private:
	PredicateTable* predicateTable;
	InstancesTable* instancesTable;
	StatementDependency* st;


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
