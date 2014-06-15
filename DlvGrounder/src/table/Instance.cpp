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



bool SimpleIndexAtom::findIfAFactExists(const AtomTable& collection,vec_pair_index_object& bound, map_int_int& equal_var) {
	vector<index_object> terms(bound.size() + equal_var.size());
	for (unsigned int i = 0; i < bound.size(); i++) {
		terms[bound[i].first] = bound[i].second;
	}
	for (auto it : equal_var) {
		terms[it.second] = terms[it.first];
	}
	index_object predicate=(*atoms->begin())->atom->getPredicate().second;
	Atom *atom=new ClassicalLiteral(predicate,terms,false,false);
	GenericAtom *genAtom=new GenericAtom;
	genAtom->atom=atom;

	bool find = collection.count(genAtom);

	delete genAtom;
	delete atom;

	return find;
}


index_object SimpleIndexAtom::firstMatch(vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,bool& find) {
	index_object id = counter;counter++;
	ResultMatch *rm = new ResultMatch(bind);

	// Call findIfAFactExist only if all the terms have value
	if(bound.size()==(*atoms->begin())->atom->getTermsSize()){
		if(findIfAFactExists(*atoms,bound,equal_var)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}
	else
		//Simple search
		computeFirstMatch(*atoms,bound,bind,equal_var,rm);

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;
}

bool SimpleIndexAtom::computeFirstMatch(const AtomTable& collection, vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,ResultMatch* rm){
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

			rm->result.insert(a);

			// If bind equal 0 check if exist one element
			// If bind equal 0 then the size of bind is not equal arity, is present anonimous
			if(bind.size()==0)break;

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

void SimpleIndexAtom::nextMatch(index_object id,vec_pair_index_object &bind,bool& find) {
	ResultMatch *rm=matches_id.find(id)->second;
	unsigned int size=rm->result.size();
	unsigned int num_variable=bind.size();

	if(size==0){
		delete rm;
		matches_id.erase(id);
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

Instances::Instances(index_object predicate) {
	this->predicate = predicate;
	switch (Config::getInstance()->getIndexType()) {
	case (IndexType::MAP):
		if(Config::getInstance()->getIndexingTerm(predicate).second)
			indexAtom = new TermIndexAtom(&facts,Config::getInstance()->getIndexingTerm(predicate).first);
		else
			indexAtom = new TermIndexAtom(&facts,0);
		break;
	case (IndexType::MULTIMAP):
		if(Config::getInstance()->getIndexingTerm(predicate).second)
				indexAtom = new TermIndexAtomMultiMap(&facts,Config::getInstance()->getIndexingTerm(predicate).first);
		else
				indexAtom = new TermIndexAtomMultiMap(&facts,0);
		break;
	default:
		indexAtom = new SimpleIndexAtom(&facts);
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


InstancesTable::~InstancesTable() {
	for (auto i : instanceTable) {
		delete (i.second);
	}
}


