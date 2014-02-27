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

long MapTermTable::addTerm(Term* t) {
	t->setIndex(counter);
	counter++;
	pair<long,Term*> pair(t->getIndex(),t);
	hash.insert(pair);

	return t->getIndex();
}

void MapTermTable::removeTerm(long index) {

}

Term* MapTermTable::getTerm(long index) {
	return hash.find(index)->second;
}

long MapTermTable::getSize() {
	return hash.size();
}

MapTermTable::~MapTermTable() {
	for(map<long,Term*>::iterator it=hash.begin();it!=hash.end();it++){
		delete (it->second);
	}
}
