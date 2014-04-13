/*
 * BoostTermTable.cpp
 *
 *  Created on: 01/mar/2014
 *      Author: Davide
 */

#include "BoostTermTable.h"
#include "IdsManager.h"

BoostTermTable::BoostTermTable() {
}

unsigned long BoostTermTable::addTerm(Term* t) {
	pair_long_bool p=IdsManager::getIndex(IdsManager::TERM_ID_MANAGER, t->getNameToHash());

	unsigned long id=p.first;
	if(!p.second){
		t->setIndex(id);
		hash.insert(t);
	}else{
		delete t;
	}
	return id;
}

void BoostTermTable::removeTerm(unsigned long index) {
}

Term* BoostTermTable::getTerm(unsigned long index) {
	Term t;
	t.setIndex(index);
	Term *termFinded=0;
	if(hash.find(&t)!=hash.end())
		termFinded=*(hash.find(&t));


	return termFinded;
}

long BoostTermTable::getSize() {
	return hash.size();
}

BoostTermTable::~BoostTermTable() {
	for(Term*t:hash)
		delete t;
}

int BoostTermTable::getCollision() {
	return IdsManager::getConflict(IdsManager::TERM_ID_MANAGER);
}

void BoostTermTable::print() {
	for(Term *t:hash)t->print();

}
