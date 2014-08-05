/*
 * Instance.cpp
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#include "Instance.h"
#include "../utility/Config.h"
#include "IdsManager.h"
//#include "../utility/Timer.h"


bool SimpleIndexAtom::findIfAFactExists(AtomTable* collection,vec_pair_index_object& bound, map_int_int& equal_var) {
	if(collection->size()==0)return false;

	//Build the ground atom using the bound value assigned, also taking into account the presence of equal variables
	vector<index_object> terms(predicate->getArity());
	for (unsigned int i = 0; i < bound.size(); i++) {
		terms[bound[i].first] = bound[i].second;
	}
	for (auto it : equal_var) {
		terms[it.second] = terms[it.first];
	}

	//Compute the hash of the atom
	GenericAtom *genAtom=new GenericAtom(terms);

	//Look for the atom
	bool find = collection->count(genAtom);

	delete genAtom;
	return find;
}


unsigned int SimpleIndexAtom::firstMatch(vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,bool& find) {
	unsigned int id = counter;counter++;
	ResultMatch *rm = new ResultMatch(bind);

	//Search in facts for match
	if(searchForFirstMatch(facts,bound,bind,equal_var,rm)){
		find=true;
		matches_id.insert({id,rm});
		return id;
	}

	//If it is EDB and not terms are bound, search also in no facts for match
	if(!predicate->isEdb()){
		if(searchForFirstMatch(nofacts,bound,bind,equal_var,rm)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;


}

bool SimpleIndexAtom::searchForFirstMatch(AtomTable* table, vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,ResultMatch* rm){

	//Call findIfAFactExist only if all the terms are bound
	if(bind.size()==0){
		if(findIfAFactExists(table,bound,equal_var)){
			return true;
		}
	}
	else
		//Compute the first match
		computeFirstMatch(table,bound,bind,equal_var,rm);
	return false;
}

void SimpleIndexAtom::computeFirstMatch(AtomTable* collection,vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,ResultMatch* rm){
	for (GenericAtom *genericAtom : *collection) {
		bool match = true;
		for (unsigned int i = 0; i < bound.size(); i++)
			if (genericAtom->terms[bound[i].first] != bound[i].second) {
				match = false;
				break;
			}
		if (match) {
			//Test the match of bind equal variable and if the terms with equal variables not match skip this atom
			if(!checkEqualVariable(equal_var,genericAtom)) continue;

			rm->result.insert(genericAtom);

			//If there are no more bind variables stop.
			//Notice that the size of the bind variables vector is not always equal to the arity of the predicate.
			//Indeed, there can be anonymous variables that are not considered to be bind.
			//The same holds for bound variables.
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

	///Return the next matching facts or no facts retrived from the integer identifier assigned by the firstMatch method

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

Instances::Instances(index_object predicate,PredicateTable *pt) {
	this->predicate = predicate;
	this->predicateTable=pt;
	Predicate *predicatePointers=predicateTable->getPredicate(predicate);
	///Properly set the IndexAtom type
	switch (Config::getInstance()->getIndexType()) {
	case (IndexType::MAP):
		if(Config::getInstance()->getIndexingTerm(predicate).second)
			indexAtom = new SingleTermIndexAtom(&facts,&nofacts,Config::getInstance()->getIndexingTerm(predicate).first,predicatePointers);
		else
			indexAtom = new SingleTermIndexAtom(&facts,&nofacts,predicatePointers);
		break;
	case (IndexType::MULTIMAP):
		if(Config::getInstance()->getIndexingTerm(predicate).second)
				indexAtom = new SingleTermIndexAtomMultiMap(&facts,&nofacts,Config::getInstance()->getIndexingTerm(predicate).first,predicatePointers);
		else
				indexAtom = new SingleTermIndexAtomMultiMap(&facts,&nofacts,predicatePointers);
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


void SingleTermIndexAtom::determineTermToBeIndexed(vec_pair_index_object& bound) {
	///If the position of the indexing term is not set by the user the first admissible term is the first bound term
	if(!positionOfIndexingSetByUser && bound.size()>0){
		positionOfIndexing = bound[0].first;
		positionOfIndexingSetByUser = true;
	}
}

void SingleTermIndexAtomMultiMap::determineTermToBeIndexed(vec_pair_index_object& bound) {
	///If the position of the indexing term is not set by the user the first admissible term is the first bound term
	if(!positionOfIndexingSetByUser && bound.size()>0){
		positionOfIndexing = bound[0].first;
		positionOfIndexingSetByUser = true;
	}
}

pair<bool, index_object> SingleTermIndexAtom::createIndex(vec_pair_index_object& bound) {
	//Determine the indexing term
	if (!positionOfIndexingSetByUser)
		determineTermToBeIndexed(bound);

	//Initialize the maps
	pair<bool, index_object> termBoundIndex( { false, 0 });
	for (unsigned int i = 0; i < bound.size(); i++)
		if (bound[i].first == positionOfIndexing) {
			termBoundIndex.first = true;
			termBoundIndex.second = bound[i].second;
			if (!instantiateIndexMaps)
				initializeIndexMaps();

			break;
		}
	return termBoundIndex;
}

pair<bool, index_object> SingleTermIndexAtomMultiMap::createIndex(vec_pair_index_object& bound) {
	//Determine the indexing term
	if (!positionOfIndexingSetByUser)
		determineTermToBeIndexed(bound);

	//Initialize the maps
	pair<bool, index_object> termBoundIndex( { false, 0 });
	for (unsigned int i = 0; i < bound.size(); i++)
		if (bound[i].first == positionOfIndexing) {
			termBoundIndex.first = true;
			termBoundIndex.second = bound[i].second;
			if (!instantiateIndexMaps)
				initializeIndexMaps();

			break;
		}
	return termBoundIndex;
}

unsigned int SingleTermIndexAtom::firstMatch(vec_pair_index_object& bound, vec_pair_index_object& bind, map_int_int& equal_var, bool& find) {

	unsigned int id = counter;counter++;
	ResultMatch* rm=new ResultMatch(bind);

	AtomTable* matchingTable;

	pair<bool, index_object> termBoundIndex = createIndex(bound);

	if(termBoundIndex.first)
		matchingTable=&factsIndexMap[termBoundIndex.second];
	else
		matchingTable=facts;

	//Search in facts for match
	if(searchForFirstMatch(matchingTable,bound,bind,equal_var,rm)){
		find=true;
		matches_id.insert({id,rm});
		return id;
	}

	//If it is EDB and it is not all bound, search also in no facts for match
	if(!predicate->isEdb() && nofacts->size()>0){
		if(termBoundIndex.first)
			matchingTable=&nofactsIndexMap[termBoundIndex.second];
		else
			matchingTable=nofacts;

		if(searchForFirstMatch(matchingTable,bound,bind,equal_var,rm)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;

}


unsigned int SingleTermIndexAtomMultiMap::firstMatch(vec_pair_index_object& bound, vec_pair_index_object& bind, map_int_int& equal_var, bool& find) {

	unsigned int id = counter;counter++;
	ResultMatch* rm=new ResultMatch(bind);

	if(bound.size()==predicate->getArity()){
		if(findIfAFactExists(facts,bound,equal_var) || findIfAFactExists(nofacts,bound,equal_var)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}
	else{

		pair<bool, index_object> termBoundIndex = createIndex(bound);

		AtomTable* matchingTable=new AtomTable;

		if(termBoundIndex.first){
			auto pair=factsIndexMap.equal_range(termBoundIndex.second);
			for(auto it=pair.first;it!=pair.second;it++)
				matchingTable->insert(it->second);
		}
		else
			matchingTable=facts;

		//Search in facts for match
		computeFirstMatch(matchingTable,bound,bind,equal_var,rm);

		//If it is EDB and it is not all bound, search also in no facts for match
		if(!predicate->isEdb() && nofacts->size()>0){

			matchingTable->clear();

			if(termBoundIndex.first){
				auto pair=nofactsIndexMap.equal_range(termBoundIndex.second);
				for(auto it=pair.first;it!=pair.second;it++)
					matchingTable->insert(it->second);
			}
			else
				matchingTable=nofacts;

			computeFirstMatch(matchingTable,bound,bind,equal_var,rm);

		}

	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;
}


void SingleTermIndexAtom::initializeIndexMaps(){
//	Timer::getInstance()->start("Creation Index Structure");
	unordered_set<index_object> termToBeIndexedIndices;

	for (GenericAtom*a : *facts) {
		index_object termIndex=a->terms[positionOfIndexing];
		if(termToBeIndexedIndices.insert(termIndex).second){
			AtomTable values;
			values.insert(a);
			factsIndexMap.insert({termIndex,values});
		}
		else{
			factsIndexMap[termIndex].insert(a);
		}
	}
	if(!predicate->isEdb()){
		termToBeIndexedIndices.clear();
		for (GenericAtom*a : *nofacts) {
			index_object termIndex=a->terms[positionOfIndexing];
			if(termToBeIndexedIndices.insert(termIndex).second){
				AtomTable values;
				values.insert(a);
				nofactsIndexMap.insert({termIndex,values});
			}
			else{
				nofactsIndexMap[termIndex].insert(a);
			}
		}
	}
//	Timer::getInstance()->end();
	instantiateIndexMaps=true;
}

void SingleTermIndexAtomMultiMap::initializeIndexMaps(){
//	Timer::getInstance()->start("Creation Index Structure");
	for (GenericAtom*a : *facts) {
		index_object termIndex=a->terms[positionOfIndexing];
		factsIndexMap.insert({termIndex,a});
	}
	if(!predicate->isEdb()){
		for (GenericAtom*a : *nofacts) {
				index_object termIndex=a->terms[positionOfIndexing];
				nofactsIndexMap.insert({termIndex,a});
		}
	}
//	Timer::getInstance()->end();
	instantiateIndexMaps=true;
}

