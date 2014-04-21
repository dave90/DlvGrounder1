/*
 * Instance.cpp
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#include "Instance.h"
#include "../utility/Config.h"
#include "IdsManager.h"

unsigned long SimpleIndexAtom::firstMatch(vector<unsigned long>& termsIndex,
		vector<unsigned long> &values, vector<unsigned long> &expected,
		vector<unsigned long> &result) {
	unsigned long id = matches_id.size();
	ResultMatch *rm = new ResultMatch;
	rm->num_variable=expected.size();
	//Simple search
	for (Atom *a : *atoms) {
		bool match = true;
		for (unsigned int i = 0; i < termsIndex.size(); i++)
			//FIXME get all term of atom
			if (a->getTerms()[termsIndex[i]] != values[i]) {
				match = false;
				break;
			}
		if (match) {
			for (unsigned long v : expected)
				rm->result.push_back(a->getTerms()[v]);
		}
	}
	matches_id.insert({id,rm});
	nextMatch(id,result);

	return id;
}

void SimpleIndexAtom::nextMatch(unsigned long id, vector<unsigned long>& result) {
	ResultMatch *rm=matches_id.find(id)->second;
	unsigned int size=rm->result.size();
	unsigned int num_variable=rm->num_variable;

	if(size==0){
		return ;
	}

	result.resize(num_variable);
	for(unsigned int i=0;i<num_variable;i++){
		result[num_variable-i-1]=rm->result.back();
		rm->result.pop_back();
	}
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

