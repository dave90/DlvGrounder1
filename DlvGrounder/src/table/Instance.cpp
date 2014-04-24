/*
 * Instance.cpp
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#include "Instance.h"
#include "../utility/Config.h"
#include "IdsManager.h"

unsigned long SimpleIndexAtom::firstMatch(vec_pair_long &bound,vec_pair_long &bind,bool& find) {
	unsigned long id = matches_id.size();
	ResultMatch *rm = new ResultMatch;


	//Simple search
	for (Atom *a : *atoms) {
		bool match = true;
		for (unsigned int i = 0; i < bound.size(); i++)
			if (a->getTerm(bound[i].first) != bound[i].second) {
				match = false;
				break;
			}
		if (match) {

			// If no bind variables but match atom finish
			if(bind.size()==0){
				find=true;
				matches_id.insert({id,rm});
				return id;
			}

			for (auto i : bind){

				rm->result.push_back(a->getTerm(i.first));
			}

		}
	}
	matches_id.insert({id,rm});


	nextMatch(id,bind,find);

	return id;
}

void SimpleIndexAtom::nextMatch(unsigned long id,vec_pair_long &bind,bool& find) {
	ResultMatch *rm=matches_id.find(id)->second;
	unsigned int size=rm->result.size();
	unsigned int num_variable=bind.size();


	if(size==0){
		find=false;
		return ;
	}


	for(unsigned int i=0;i<num_variable;i++){
		bind[num_variable-i-1].second=rm->result.back();
		rm->result.pop_back();
	}
	find=true;
}

SimpleIndexAtom::~SimpleIndexAtom() {
}

Instances::Instances(unsigned long predicate) {
	this->predicate = predicate;
	switch (Config::getInstance()->getIndexType()) {
	default:
		//FIXME NOT only fact
		indexAtom = new SimpleIndexAtom(&facts);
		break;
	}
}

Instances::~Instances() {
	for (auto it = facts.begin(); it != facts.end(); it++)
		delete (*it);
	for (auto it = nofacts.begin(); it != nofacts.end(); it++)
		delete (it->atom);
	delete (indexAtom);
}
;

void Instances::computeAtomIndex(Atom*& a) {
	pair<unsigned long, bool> index = IdsManager::getIndex(IdsManager::ATOM_ID_MANAGER,
			a->getNameToHash());
	a->setIndex(index.first);
}

InstancesTable::~InstancesTable() {
	for (Instances* i : instanceTable) {
		delete i;
	}
}

void Instances::print() {
	cout << "Predicate " << predicate << endl;
	for (Atom* a : facts) {
		a->print();
	}
}

