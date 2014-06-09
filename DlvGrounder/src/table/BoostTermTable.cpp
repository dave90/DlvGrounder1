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

index_object BoostTermTable::addTerm(Term* t) {
	string hashString=t->getNameToHash();
	pair_long_bool p=IdsManager::getIndex(IdsManager::TERM_ID_MANAGER, hashString);

	index_object id=p.first;
	if(!p.second){
		t->setIndex(id);
		hash.insert(t);
	}else{
		delete t;
	}
	return id;
}

index_object BoostTermTable::addTerm(Term* t,string &s) {
	pair_long_bool p=IdsManager::getIndex(IdsManager::TERM_ID_MANAGER, s);

	index_object id=p.first;
	if(!p.second){
		t->setIndex(id);
		hash.insert(t);
	}else{
		delete t;
	}
	return id;
}

index_object BoostTermTable::addTerm(Term* t,unsigned int &s) {
	pair_long_bool p=IdsManager::getIndex(IdsManager::TERM_ID_MANAGER, s);

	index_object id=p.first;
	if(!p.second){
		t->setIndex(id);
		hash.insert(t);
	}else{
		delete t;
	}
	return id;
}

void BoostTermTable::removeTerm(index_object index) {
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
