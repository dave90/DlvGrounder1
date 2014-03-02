/*
 * BoostTermTable.cpp
 *
 *  Created on: 01/mar/2014
 *      Author: Davide
 */

#include "BoostTermTable.h"

BoostTermTable::BoostTermTable() {
	counter=0;
}

unsigned long BoostTermTable::addTerm(Term* t) {
	unsigned long index;

	if(!nameToIndex.count(t->getName())){
		t->setIndex(counter);
		counter++;

		hash.insert(t);
		index=t->getIndex();

		pair<string,unsigned long> p2(t->getName(),index);
		nameToIndex.insert(p2);

	}else{
		auto its = nameToIndex.equal_range(t->getName());
		for (auto it = its.first; it != its.second; ++it) {
		    if(strcmp(t->getName().c_str(),it->first.c_str())==0){
		    	index=it->second;
		    	break;
		    }
		}

		delete t;
	}

	return index;
}

void BoostTermTable::removeTerm(unsigned long index) {
}

Term* BoostTermTable::getTerm(unsigned long index) {
	Term *t=new Term;
	t->setIndex(index);
	Term *termFinded=0;
	if(hash.find(t)!=hash.end())
		termFinded=*(hash.find(t));


	return termFinded;
}

long BoostTermTable::getSize() {
	return hash.size();
}

BoostTermTable::~BoostTermTable() {
	for(boost::unordered_set<Term*>::iterator it=hash.begin();it!=hash.end();it++)
		delete (*it);
}
