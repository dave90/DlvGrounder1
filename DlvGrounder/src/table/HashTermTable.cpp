/*
 * HashTermTable.cpp
 *
 *  Created on: 01/mar/2014
 *      Author: Davide
 */

#include "HashTermTable.h"
#include "IdsManager.h"

HashTermTable::HashTermTable() {
}

unsigned long HashTermTable::addTerm(Term* t) {
	string hashString=t->getNameToHash();
	pair_long_bool p=IdsManager::getIndex(IdsManager::TERM_ID_MANAGER, hashString);
	unsigned long id=p.first;
	if(!p.second){
		t->setIndex(id);
		hash.insert(t);
	}else{
		delete t;
	}
	return id;

}


unsigned long HashTermTable::addTerm(Term* t,string &s) {
	pair_long_bool p=IdsManager::getIndex(IdsManager::TERM_ID_MANAGER, s);

	unsigned long id=p.first;
	if(!p.second){
		t->setIndex(id);
		hash.insert(t);
	}else{
		delete t;
	}
	return id;
}

unsigned long HashTermTable::addTerm(Term* t,unsigned int &s) {
	pair_long_bool p=IdsManager::getIndex(IdsManager::TERM_ID_MANAGER, s);

	unsigned long id=p.first;
	if(!p.second){
		t->setIndex(id);
		hash.insert(t);
	}else{
		delete t;
	}
	return id;
}


void HashTermTable::removeTerm(unsigned long index) {
}

Term* HashTermTable::getTerm(unsigned long index) {
	Term t;
	t.setIndex(index);
	Term *termFinded=0;
	if(hash.find(&t)!=hash.end())
		termFinded=*(hash.find(&t));


	return termFinded;
}

long HashTermTable::getSize() {
	return hash.size();
}

int HashTermTable::getCollision() {
	return IdsManager::getConflict(IdsManager::TERM_ID_MANAGER);
}


HashTermTable::~HashTermTable() {
	for(Term*t:hash)
		delete t;

}
