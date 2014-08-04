/*
 * BoostTermTable.cpp
 *
 *  Created on: 01/mar/2014
 *      Author: Davide
 */

#include "BoostTermTable.h"
#include "IdsManager.h"


void BoostTermTable::insertTermByIndex(index_object &id,bool insert,Term* t) {
	if (insert) {
		// Term t is not present
		t->setIndex(id);
		hash.insert(t);
	} else {
		// Term t already exist
		delete t;
	}
}

index_object BoostTermTable::addTerm(Term* t) {
	string hashString=t->getNameToHash();

	return addTerm(t,hashString);
}


index_object BoostTermTable::addTerm(Term* t,string &s) {
	pair_long_bool index_bool=IdsManager::getIndex(IdsManager::TERM_ID_MANAGER, s);

	insertTermByIndex(index_bool.first,!index_bool.second, t);

	return index_bool.first;
}

index_object BoostTermTable::addTerm(Term* t,unsigned int &s) {
	pair_long_bool index_bool=IdsManager::getIndex(IdsManager::TERM_ID_MANAGER, s);

	insertTermByIndex(index_bool.first,!index_bool.second, t);

	return index_bool.first;
}


Term* BoostTermTable::getTerm(index_object index) {
	Term t;
	t.setIndex(index);
	Term *termFinded=0;
	if(hash.find(&t)!=hash.end())
		termFinded=*(hash.find(&t));


	return termFinded;
}

unsigned int BoostTermTable::getSize() {
	return hash.size();
}

BoostTermTable::~BoostTermTable() {
	for(Term*t:hash)
		delete t;
}

int BoostTermTable::getCollision() {
	return IdsManager::getConflict(IdsManager::TERM_ID_MANAGER);
}
