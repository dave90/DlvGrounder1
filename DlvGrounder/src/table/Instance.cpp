/*
 * Instance.cpp
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#include "Instance.h"
#include "../utility/Config.h"
//#include "../utility/Timer.h"

/****************************************************** INSTANCES ***************************************************/

void Instance::setIndexAtom(){
	// Properly set the IndexAtom type
	switch (Config::getInstance()->getIndexType()) {
	case (IndexType::MAP):
		indexAtom = new SingleTermIndexAtom(&tables[Instance::FACTS],&tables[Instance::NOFACTS],&tables[Instance::DELTA],predicate);
		break;
	case (IndexType::MULTIMAP):
		indexAtom = new SingleTermIndexAtomMultiMap(&tables[Instance::FACTS],&tables[Instance::NOFACTS],&tables[Instance::DELTA],predicate);
		break;
	default:
		indexAtom = new SimpleIndexAtom(&tables[Instance::FACTS],&tables[Instance::NOFACTS],&tables[Instance::DELTA],predicate);
		break;
	}
}

Instance::~Instance() {
	for(unsigned int i=0;i<tables.size();i++){
		AtomTable* table=&tables[i];
		for (auto it = table->begin(); it != table->end(); it++){
			delete *it;
		}
	}
	delete (indexAtom);
}

void Instance::moveNextDeltaInDelta(){
	AtomTable* nofacts=&tables[Instance::NOFACTS];
	AtomTable* delta=&tables[Instance::DELTA];
	AtomTable* nextDelta=&tables[Instance::NEXTDELTA];
	if(delta->size()>0){
		for(GenericAtom* atom: *delta)
				nofacts->insert(atom);
		delta->clear();
	}
	if(nextDelta->size()>0){
		indexAtom->updateDelta(nextDelta);
		for(GenericAtom* atom: *nextDelta)
			delta->insert(atom);
		nextDelta->clear();
	}
}

bool Instance::isTrue(vector<Term*>& terms) {
	GenericAtom* atom=getGenericAtom(Instance::NEXTDELTA,terms,0);
	return atom->isFact();
}

void Instance::setTruth(vector<Term*>& terms, bool truth) {
	GenericAtom* atom=getGenericAtom(Instance::NEXTDELTA,terms,truth);
	atom->setFact(truth);
}

GenericAtom* Instance::getGenericAtom(unsigned int table,vector<Term*>& terms,bool truth) {
	GenericAtom* atom=new AtomUndef(terms,truth);
	for(unsigned int i=Instance::FACTS;i<table;i++){
		auto it=tables[i].find(atom);
		if(it!=tables[i].end()){
			GenericAtom* tmp=atom;
			atom = (*it);
			delete tmp;
			return atom;
		}
	}
	delete atom;
	return 0;
}

GenericAtom* Instance::getGenericAtom(vector<Term*>& terms,bool truth) {
	return getGenericAtom(Instance::NEXTDELTA,terms,truth);
}

bool Instance::add(unsigned int table, GenericAtom*& atomUndef, bool& updated) {
	GenericAtom* atom=getGenericAtom(table+1,atomUndef->terms,atomUndef->isFact());
	if(atom==0){
		tables[table].insert(atomUndef);
		return true;
	}
	if(atomUndef->isFact() && !atom->isFact()){
		atom->setFact(true);
		updated=true;
		return false;
	}
	return false;
}

/****************************************************** INSTANCES TABLE ***************************************************/

InstanceTable::~InstanceTable() {
	for (auto i : instanceTable)
		delete (i.second);
}



