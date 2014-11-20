/*
 * TermTable.cpp
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#include "TermTable.h"


TermTable *TermTable::termTable;

Term* TermTable::addTerm(Term* t) {
	return terms.insert(t);
}

Term* TermTable::getTerm(Term* t) {
	return terms.get(t);
}

unsigned int TermTable::getSize() {
	return terms.flyweight_set.size();
}

int TermTable::getCollision() {
	return 0;
}

TermTable* TermTable::getInstance() {

	if (termTable == nullptr)
		termTable=new TermTable;

	return termTable;
}


