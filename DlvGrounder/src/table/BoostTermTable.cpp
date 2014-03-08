/*
 * BoostTermTable.cpp
 *
 *  Created on: 01/mar/2014
 *      Author: Davide
 */

#include "BoostTermTable.h"

BoostTermTable::BoostTermTable() {
}

unsigned long BoostTermTable::addTerm(Term* t) {
	pair_long_bool p=idManager.insert(t->getNameToHash());

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
	for(boost::unordered_set<Term*,hashTerm,equalTerm>::iterator it=hash.begin();it!=hash.end();it++)
		delete (*it);
}
