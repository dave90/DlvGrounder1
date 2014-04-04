/*
 * Instance.cpp
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#include "Instance.h"
#include "../utility/Config.h"
#include "IdsManager.h"

Instances::Instances(unsigned long predicate) {
	this->predicate=predicate;
	switch(Config::getInstance()->getIndexType()){
		default:
			indexAtom=0; //FIXME
			break;
	}
}

Instances::~Instances(){
	for(auto it = facts.begin(); it!= facts.end(); it++) delete(*it);
	for(auto it = nofacts.begin(); it!= nofacts.end(); it++) delete(it->atom);
	delete(indexAtom);
};

void Instances::computeAtomIndex(Atom*& a){
	pair<unsigned long, bool> index = IdsManager::getIndex(IdsManager::ATOM_ID_MANAGER, a->getNameToHash());
	a->setIndex(index.first);
}

InstancesTable::~InstancesTable() {
	for(Instances* i:instanceTable){delete i;}
}

void Instances::print() {
	cout<<"Predicate "<<predicate<<endl;
	for(Atom* a:facts){
		a->print();
	}
}
