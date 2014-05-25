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

#include <boost/lexical_cast.hpp>

bool SimpleIndexAtom::isPresent(unordered_set<string> &result_string,string result_terms){
	if(result_string.count(result_terms))
		return true;
	result_string.insert(result_terms);
	return false;
}

unsigned long SimpleIndexAtom::firstMatch(vec_pair_long &bound,vec_pair_long &bind,map_int_int& equal_var,bool& find) {
	unsigned long id = matches_id.size();
	ResultMatch *rm = new ResultMatch;
	unordered_set<string> result_string;

	//Simple search
	if(computeFirstMatch(*atoms,bound,bind,equal_var,rm,result_string)){
		find=true;
		matches_id.insert({id,rm});
		return id;
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;
}

bool SimpleIndexAtom::computeFirstMatch(const AtomTable& collection, vec_pair_long &bound,vec_pair_long &bind,map_int_int& equal_var,ResultMatch* rm,
		unordered_set<string> result_string){
	for (Atom *a : collection) {
		bool match = true;
		for (unsigned int i = 0; i < bound.size(); i++)
			if (a->getTerm(bound[i].first) != bound[i].second) {
				match = false;
				break;
			}
		if (match) {

			//Test the match of bind equal variable
			bool skipAtom=false;
			for(auto it:equal_var){
				if(a->getTerm(it.first) != a->getTerm(it.second)) {
					skipAtom=true;
					break;
				}
			}
			// If the term with equal variable not match skip this atom
			if(skipAtom)continue;
			// If no bind variables but match atom finish
			if(bind.size()==0){
				return true;
			}

			string result_terms="";
			for (auto i : bind){
				result_terms+=boost::lexical_cast<string>(a->getTerm(i.first))+"*";
			}

			if(!isPresent(result_string,result_terms))
				rm->result.insert(a);
		}
	}
	return false;
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

	AtomTable::iterator it_last_atom=rm->result.begin();
	for(unsigned int i=0;i<num_variable;i++){
		bind[i].second=(*it_last_atom)-> getTerm(bind[i].first);
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
//		indexAtom = new TermIndexAtom(&facts,Config::getInstance()->getIndexingTerm(predicate));
		indexAtom = new TermIndexAtomMultiMap(&facts,Config::getInstance()->getIndexingTerm(predicate));
//		indexAtom = new SimpleIndexAtom(&facts);
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


