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

	vector<index_object> terms(bound.size() + equal_var.size());
	for (unsigned int i = 0; i < bound.size(); i++) {
		terms[bound[i].first] = bound[i].second;
	}
	for (auto it : equal_var) {
		terms[it.second] = terms[it.first];
	}

	index_object predicate=(*collection.begin())->atom->getPredicate().second;


	Atom *atom=new ClassicalLiteral(predicate,terms,false,false);
	GenericAtom *genAtom=new GenericAtom;
	genAtom->atom=atom;

	bool find = collection.count(genAtom);

	delete genAtom;
	delete atom;

	return find;
}


unsigned int SimpleIndexAtom::firstMatch(bool isEDB,vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,bool& find) {
	unsigned int id = counter;counter++;
	ResultMatch *rm = new ResultMatch(bind);
	unsigned int atomTermSize=0;
	if(facts->size()>0)
		atomTermSize=(*facts->begin())->atom->getTermsSize();
	else
		atomTermSize=(*nofacts->begin())->atom->getTermsSize();

	if(!isEDB){
		find=searchForFirstMatch(facts,atomTermSize,bound,bind,equal_var,rm);
		if(searchForFirstMatch(nofacts,atomTermSize,bound,bind,equal_var,rm))
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
		if(searchForFirstMatch(facts,atomTermSize,bound,bind,equal_var,rm)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
		matches_id.insert({id,rm});
		nextMatch(id,bind,find);
		return id;
	}


}

bool SimpleIndexAtom::searchForFirstMatch(AtomTable* table, const unsigned int atomTermSize, vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,ResultMatch* rm){

	// Call findIfAFactExist only if all the terms have value
	if(bound.size()==atomTermSize){
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
}

bool SimpleIndexAtom::checkEqualVariable(map_int_int& equal_var,Atom *atom){
	for(auto it:equal_var){
		if(atom->getTerm(it.first) != atom->getTerm(it.second)) {
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
			indexAtom = new TermIndexAtom(&facts,&nofacts,Config::getInstance()->getIndexingTerm(predicate).first);
		else
			indexAtom = new TermIndexAtom(&facts,&nofacts);
		break;
	case (IndexType::MULTIMAP):
		if(Config::getInstance()->getIndexingTerm(predicate).second)
				indexAtom = new TermIndexAtomMultiMap(&facts,&nofacts,Config::getInstance()->getIndexingTerm(predicate).first);
		else
				indexAtom = new TermIndexAtomMultiMap(&facts,&nofacts);
		break;
	default:
		indexAtom = new SimpleIndexAtom(&facts,&nofacts);
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


