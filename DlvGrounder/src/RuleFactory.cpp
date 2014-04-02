/*
 * AtomFactory.cpp
 *
 *  Created on: 27/mar/2014
 *      Author: Davide
 */

#include "RuleFactory.h"

#include "atom/ClassicalLiteral.h"

RuleFactory::RuleFactory() {
	head=true;
}

void RuleFactory::addPredicate(string name) {
	lastPredicate=name;
}

void RuleFactory::addGClassicalLiteral(vector<unsigned long> terms,
		bool hashMinus, bool negative) {
	Predicate p(lastPredicate,terms.size());
	unsigned long index=predicateTable.insertPredicate(p);
	instancesTable.addInstance(index);
	Atom* atom=new ClassicalLiteral(index,terms,hashMinus,negative);
	instancesTable.getInstance(index)->addFact(atom);

}

void RuleFactory::print() {
	instancesTable.print();
}
