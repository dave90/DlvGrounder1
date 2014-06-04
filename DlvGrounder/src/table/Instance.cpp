/*
 * Instance.cpp
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#include "Instance.h"
#include "../utility/Config.h"
#include "IdsManager.h"
#include "TermIndexAtom.h"



unsigned long SimpleIndexAtom::firstMatch(vec_pair_long &bound,vec_pair_long &bind,map_int_int& equal_var,bool& find) {
	unsigned long id = matches_id.size();
	ResultMatch *rm = new ResultMatch(bind);

	//Simple search
	if(computeFirstMatch(*atoms,bound,bind,equal_var,rm)){
		find=true;
		matches_id.insert({id,rm});
		return id;
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;
}

bool SimpleIndexAtom::computeFirstMatch(const AtomTable& collection, vec_pair_long &bound,vec_pair_long &bind,map_int_int& equal_var,ResultMatch* rm){
	for (GenericAtom *genericAtom : collection) {
		Atom *a=genericAtom->atom;

		bool match = true;
		for (unsigned int i = 0; i < bound.size(); i++)
			if (a->getTerm(bound[i].first) != bound[i].second) {
				match = false;
				break;
			}
		if (match) {

			//Test the match of bind equal variable
			// and If the term with equal variable not match skip this atom
			if(!checkEqualVariable(equal_var,a))continue;
			// If no bind variables but match atom finish
			if(bind.size()==0){
				return true;
			}

			rm->result.insert(a);

		}
	}
	return false;
}

bool SimpleIndexAtom::checkEqualVariable(map_int_int& equal_var,Atom *atom){
	for(auto it:equal_var){
		if(atom->getTerm(it.first) != atom->getTerm(it.second)) {
			return false;
		}
	}
	return true;
}

void SimpleIndexAtom::nextMatch(unsigned long id,vec_pair_long &bind,bool& find) {
	ResultMatch *rm=matches_id.find(id)->second;
	unsigned int size=rm->result.size();
	unsigned int num_variable=bind.size();


	if(size==0){
		delete rm;
		find=false;
		return ;
	}

	auto it_last_atom=rm->result.begin();
	for(unsigned int i=0;i<num_variable;i++){
		bind[i].second=(*it_last_atom)->getTerm(bind[i].first);
	}
	find=true;
	rm->result.erase(it_last_atom);
}

SimpleIndexAtom::~SimpleIndexAtom() {}

Instances::Instances(unsigned long predicate) {
	this->predicate = predicate;
	switch (Config::getInstance()->getIndexType()) {
	default:
		//FIXME NOT only fact
		indexAtom = new TermIndexAtom(&facts,Config::getInstance()->getIndexingTerm(predicate));
//		indexAtom = new TermIndexAtomMultiMap(&facts,Config::getInstance()->getIndexingTerm(predicate));
//		indexAtom = new SimpleIndexAtom(&facts);
		break;
	}
}

Instances::~Instances() {
	for (auto it = facts.begin(); it != facts.end(); it++){
		delete (*it)->atom;
		delete *it;
	}
	for (auto it = nofacts.begin(); it != nofacts.end(); it++){
		delete (*it)->atom;
		delete *it;
	}
	delete (indexAtom);
}

void Instances::computeAtomIndex(Atom*& a) {
	string nameHash=a->getNameToHash();
	pair<unsigned long, bool> index = IdsManager::getIndex(IdsManager::ATOM_ID_MANAGER,nameHash);
	a->setIndex(index.first);
}

InstancesTable::~InstancesTable() {
	for (Instances* i : instanceTable) {
		delete i;
	}
}


