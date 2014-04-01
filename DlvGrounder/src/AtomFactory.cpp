/*
 * AtomFactory.cpp
 *
 *  Created on: 27/mar/2014
 *      Author: Davide
 */

#include "AtomFactory.h"

#include "atom/ClassicalLiteral.h"

AtomFactory::AtomFactory() {
	// TODO Auto-generated constructor stub

}

void AtomFactory::addPredicate(string name) {
	lastPredicate=name;
}

void AtomFactory::addGClassicalLiteral(vector<unsigned long> terms,
		bool hashMinus, bool negative) {
	Predicate p(lastPredicate,terms.size());
	unsigned long index=predicateTable.insertPredicate(p);
	instancesTable.addInstance(index);
	Atom* atom=new ClassicalLiteral(index,terms,hashMinus,negative);
	Instances *i=instancesTable.getInstance(index);
	i->addFact(atom);

}
