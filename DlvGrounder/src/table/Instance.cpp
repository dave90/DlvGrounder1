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
	if(collection.size()==0)return false;

	vector<index_object> terms(predicate->getArity());
	for (unsigned int i = 0; i < bound.size(); i++) {
		terms[bound[i].first] = bound[i].second;
	}
	for (auto it : equal_var) {
		terms[it.second] = terms[it.first];
	}


	GenericAtom *genAtom=new GenericAtom(terms);

	bool find = collection.count(genAtom);

	delete genAtom;
	return find;
}


unsigned int SimpleIndexAtom::firstMatch(vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,bool& find) {
	unsigned int id = counter;counter++;
	ResultMatch *rm = new ResultMatch(bind);


	if(!predicate->isEdb()){
		find=searchForFirstMatch(facts,bound,bind,equal_var,rm);
		if(searchForFirstMatch(nofacts,bound,bind,equal_var,rm))
			find=true;
		if(find){
			matches_id.insert({id,rm});
			return id;
		}
		matches_id.insert({id,rm});
		nextMatch(id,bind,find);
		return id;
	}
	else{
		if(searchForFirstMatch(facts,bound,bind,equal_var,rm)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
		matches_id.insert({id,rm});
		nextMatch(id,bind,find);
		return id;
	}


}

bool SimpleIndexAtom::searchForFirstMatch(AtomTable* table,  vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,ResultMatch* rm){

	// Call findIfAFactExist only if all the terms have value
	if(bound.size()==predicate->getArity()){
		if(findIfAFactExists(*table,bound,equal_var)){
			return true;
		}
	}
	else
		//Simple search
		computeFirstMatch(*table,bound,bind,equal_var,rm);
	return false;
}

void SimpleIndexAtom::computeFirstMatch(const AtomTable& collection, vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,ResultMatch* rm){
	for (GenericAtom *genericAtom : collection) {
		bool match = true;
		for (unsigned int i = 0; i < bound.size(); i++)
			if (genericAtom->terms[bound[i].first] != bound[i].second) {
				match = false;
				break;
			}
		if (match) {

			//Test the match of bind equal variable
			// and If the term with equal variable not match skip this atom
			if(!checkEqualVariable(equal_var,genericAtom))continue;
			// If no bind variables but match atom finish

			rm->result.insert(genericAtom);

			// If bind equal 0 check if exist one element
			// If bind equal 0 then the size of bind is not equal arity, is present anonimous
			if(bind.size()==0)break;

		}
	}
}

bool SimpleIndexAtom::checkEqualVariable(map_int_int& equal_var,GenericAtom *atom){
	for(auto it:equal_var){
		if(atom->terms[it.first] != atom->terms[it.second] ) {
			return false;
		}
	}
	return true;
}

void SimpleIndexAtom::nextMatch(unsigned int id,vec_pair_index_object &bind,bool& find) {
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
		bind[i].second=(*it_last_atom)->terms[bind[i].first];
	}
	find=true;
	rm->result.erase(it_last_atom);
}

SimpleIndexAtom::~SimpleIndexAtom() {}

Instances::Instances(index_object predicate,PredicateTable *pt) {
	this->predicate = predicate;
	this->predicateTable=pt;
	Predicate *predicatePointers=predicateTable->getPredicate(predicate);
	switch (Config::getInstance()->getIndexType()) {
	case (IndexType::MAP):
		if(Config::getInstance()->getIndexingTerm(predicate).second)
			indexAtom = new TermIndexAtom(&facts,&nofacts,Config::getInstance()->getIndexingTerm(predicate).first,predicatePointers);
		else
			indexAtom = new TermIndexAtom(&facts,&nofacts,predicatePointers);
		break;
	case (IndexType::MULTIMAP):
		if(Config::getInstance()->getIndexingTerm(predicate).second)
				indexAtom = new TermIndexAtomMultiMap(&facts,&nofacts,Config::getInstance()->getIndexingTerm(predicate).first,predicatePointers);
		else
				indexAtom = new TermIndexAtomMultiMap(&facts,&nofacts,predicatePointers);
		break;
	default:
		indexAtom = new SimpleIndexAtom(&facts,&nofacts,predicatePointers);
		break;
	}
}

Instances::~Instances() {
	for (auto it = facts.begin(); it != facts.end(); it++){
		delete *it;
	}
	for (auto it = nofacts.begin(); it != nofacts.end(); it++){
		delete *it;
	}
	delete (indexAtom);
}


InstancesTable::~InstancesTable() {
	for (auto i : instanceTable) {
		delete (i.second);
	}
}


