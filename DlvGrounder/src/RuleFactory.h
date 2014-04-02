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

class RuleFactory {
public:
	RuleFactory();
	// Method for creation of Atom
	void addPredicate(string name);
	// Add atom in rule
	void addAtom(vector<unsigned long> terms,bool hashMinus,bool negative);
	// Add Fact in Instances
	void addGClassicalLiteral(vector<unsigned long> terms,bool hashMinus,bool negative);
	// Set head false and put the next atoms in the body
	void setAtomInHead(bool b);

	void print();

private:
	PredicateTable predicateTable;
	InstancesTable instancesTable;

	/*
	 *  Variable for parsing atom
	 */

	string lastPredicate;

	/*
	 *  Variable for parsing rule
	 */
	bool head;
};

#endif /* ATOMFACTORY_H_ */
