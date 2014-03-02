/*
 * MapTermTable.cpp
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#include "MapTermTable.h"

MapTermTable::MapTermTable() {
	counter=0;
}

unsigned long MapTermTable::addTerm(Term* t) {

	unsigned long index;

	if(!nameToIndex.count(t->getName())){
		t->setIndex(counter);
		counter++;

		pair<unsigned long,Term*> p1(t->getIndex(),t);
		hash.insert(p1);
		index=t->getIndex();

		pair<string,unsigned long> p2(t->getName(),index);
		nameToIndex.insert(p2);

	}else{
		index=nameToIndex.find(t->getName())->second;
		delete t;
	}

	return index;
}

void MapTermTable::removeTerm(unsigned long index) {

}

Term* MapTermTable::getTerm(unsigned long index) {
	return hash.find(index)->second;
}

long MapTermTable::getSize() {
	return hash.size();
}

MapTermTable::~MapTermTable() {
	for(map<unsigned long,Term*>::iterator it=hash.begin();it!=hash.end();it++){
		delete (it->second);
	}
}
