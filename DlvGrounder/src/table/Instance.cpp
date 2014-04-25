/*
 * Instance.cpp
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#include "Instance.h"
#include "../utility/Config.h"
#include "IdsManager.h"

#include <boost/lexical_cast.hpp>

bool isPresent(unordered_set<string> &result_string,string result_terms){
	if(result_string.count(result_terms))
		return true;
	result_string.insert(result_terms);
	return false;
}

unsigned long SimpleIndexAtom::firstMatch(vec_pair_long &bound,vec_pair_long &bind,bool& find) {
	unsigned long id = matches_id.size();
	ResultMatch *rm = new ResultMatch;

	unordered_set<string> result_string;


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

			string result_terms="";
			vector<unsigned long> result_term_id;
			for (auto i : bind){
				result_term_id.push_back(a->getTerm(i.first));
				result_terms+=boost::lexical_cast<string>(a->getTerm(i.first))+"*";
			}

			if(!isPresent(result_string,result_terms))
				rm->result.push_back(a);

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
		delete rm;
		find=false;
		return ;
	}

	Atom *last_atom=rm->result.back();
	rm->result.pop_back();
	for(unsigned int i=0;i<num_variable;i++){
		bind[i].second=last_atom->getTerm(bind[i].first);
	}
	find=true;
}


void SimpleIndexAtom::hashAtoms(Atom_Match_Set& set) {

	//Simple hashing atoms
	for (Atom *a : *atoms) {
		set.insert(a);
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

