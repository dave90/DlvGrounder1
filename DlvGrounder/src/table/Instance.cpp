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

bool IndexAtom::match(GenericAtom *genericAtom,Atom *templateAtom,map_index_index& currentAssignment){
	// Call match for each term and if all term result true put the assignment in the current assignment


	TermTable *table=TermTable::getInstance();
	unordered_map<index_object, index_object> assignInTerm(currentAssignment);

	for(int i=0;i<genericAtom->terms.size();i++)
		if(!table->getTerm(genericAtom->terms[i])->match(templateAtom->getTerm(i).second,assignInTerm))
			return false;

	for(auto assignment:assignInTerm)currentAssignment.insert(assignment);
	return true;
}

bool IndexAtom::match(GenericAtom *genericAtom,Atom *templateAtom){
	// Call match for each term and if all term result true put the assignment in the current assignment

	TermTable *table=TermTable::getInstance();
	unordered_map<index_object, index_object> assignInTerm;

	for(int i=0;i<genericAtom->terms.size();i++)
		if(!table->getTerm(genericAtom->terms[i])->match(templateAtom->getTerm(i).second,assignInTerm))
			return false;

	return true;
}


bool SimpleIndexAtom::findIfAFactExists(AtomTable* collection) {
	if(collection->size()==0)return false;

	//Compute the hash of the atom
	GenericAtom *genAtom=new GenericAtom(templateAtom->getTerms());

	//Look for the atom
	bool find = collection->count(genAtom);

	delete genAtom;
	return find;
}


unsigned int SimpleIndexAtom::firstMatch(Atom *templateAtom,map_index_index& currentAssignment, bool& find) {
	unsigned int id = counter;counter++;

	this->templateAtom=templateAtom;
	this->currentAssignment=&currentAssignment;

	// Hash the bind term, the variable term or term that contain variable term
	vector<unsigned int> bind;
	for(unsigned int i=0;i<templateAtom->getTermsSize();i++)
		if(TermTable::getInstance()->getTerm(templateAtom->getTerm(i).second)->isVariable())bind.push_back(i);
	ResultMatch *rm = new ResultMatch(bind);

	//Search in facts for match
	if(searchForFirstMatch(facts,rm)){
		find=true;
		matches_id.insert({id,rm});
		return id;
	}

	//If it is EDB and not terms are bound, search also in no facts for match
	if(!predicate->isEdb()){
		if(searchForFirstMatch(nofacts,rm)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}

	matches_id.insert({id,rm});
	nextMatch(id,templateAtom,currentAssignment,find);
	return id;


}

bool SimpleIndexAtom::searchForFirstMatch(AtomTable* table, ResultMatch* rm){

	//Call findIfAFactExist only if all the terms are bound
	if(templateAtom->isGround()){
		if(findIfAFactExists(table)){
			return true;
		}
	}
	else
		//Compute the first match
		computeFirstMatch(table,rm);
	return false;
}

void SimpleIndexAtom::computeFirstMatch(AtomTable* collection,ResultMatch* rm){
	for (GenericAtom *genericAtom : *collection) {

		if (match(genericAtom,templateAtom)) {
			rm->result.insert(genericAtom);

			//If there are no more bind variables stop.
			//Notice that the size of the bind variables vector is not always equal to the arity of the predicate.
			//Indeed, there can be anonymous variables that are not considered to be bind.
			//The same holds for bound variables.
			if(templateAtom->isGround())break;

		}
	}
}


void SimpleIndexAtom::nextMatch(unsigned int id,Atom *templateAtom,map_index_index& currentAssignment,bool& find) {
	ResultMatch *rm=matches_id.find(id)->second;
	unsigned int size=rm->result.size();

	///Return the next matching facts or no facts retrived from the integer identifier assigned by the firstMatch method
	if(size==0){
		delete rm;
		matches_id.erase(id);
		find=false;
		return ;
	}

	auto it_last_atom=rm->result.begin();

	//Insert the value of variable in assignment
	match(*it_last_atom,templateAtom,currentAssignment);

	find=true;
	rm->result.erase(it_last_atom);
}

Instances::Instances(index_object predicate,PredicateTable *pt) {
	this->predicate = predicate;
	this->predicateTable=pt;
	Predicate *predicatePointer=predicateTable->getPredicate(predicate);
	///Properly set the IndexAtom type
	switch (Config::getInstance()->getIndexType()) {
	case (IndexType::MAP):
		if(Config::getInstance()->getIndexingTerm(predicate).second)
			indexAtom = new SingleTermIndexAtom(&facts,&nofacts,Config::getInstance()->getIndexingTerm(predicate).first,predicatePointer);
		else
			indexAtom = new SingleTermIndexAtom(&facts,&nofacts,predicatePointer);
		break;
	case (IndexType::MULTIMAP):
		if(Config::getInstance()->getIndexingTerm(predicate).second)
				indexAtom = new SingleTermIndexAtomMultiMap(&facts,&nofacts,Config::getInstance()->getIndexingTerm(predicate).first,predicatePointer);
		else
				indexAtom = new SingleTermIndexAtomMultiMap(&facts,&nofacts,predicatePointer);
		break;
	default:
		indexAtom = new SimpleIndexAtom(&facts,&nofacts,predicatePointer);
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

//unsigned int SingleTermIndexAtom::firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,vec_pair_index_object& boundFunction,vec_pair_index_object bindFunction, map_int_int& equal_var,bool& find) {
//
//	unsigned int id = counter;counter++;
//	ResultMatch* rm=new ResultMatch(bind);
//
//	AtomTable* matchingTable;
//
//	pair<bool, index_object> termBoundIndex = createIndex(bound);
//
//	if(termBoundIndex.first)
//		matchingTable=&factsIndexMap[termBoundIndex.second];
//	else
//		matchingTable=facts;
//
//	//Search in facts for match
//	if(searchForFirstMatch(matchingTable,bound,bind,boundFunction,bindFunction,equal_var,rm)){
//		find=true;
//		matches_id.insert({id,rm});
//		return id;
//	}
//
//	//If it is EDB and it is not all bound, search also in no facts for match
//	if(!predicate->isEdb() && nofacts->size()>0){
//		if(termBoundIndex.first)
//			matchingTable=&nofactsIndexMap[termBoundIndex.second];
//		else
//			matchingTable=nofacts;
//
//		if(searchForFirstMatch(matchingTable,bound,bind,boundFunction,bindFunction,equal_var,rm)){
//			find=true;
//			matches_id.insert({id,rm});
//			return id;
//		}
//	}
//
//	matches_id.insert({id,rm});
//	nextMatch(id,bind,boundFunction,bindFunction,find);
//	return id;
//
//}


//unsigned int SingleTermIndexAtomMultiMap::firstMatch(vec_pair_index_object& bound, vec_pair_index_object& bind,vec_pair_index_object& boundFunction,vec_pair_index_object &bindFunction, map_int_int& equal_var, bool& find) {
//
//	unsigned int id = counter;counter++;
//	ResultMatch* rm=new ResultMatch(bind);
//
//	if(bound.size()==predicate->getArity()){
//		if(findIfAFactExists(facts,bound,equal_var) || findIfAFactExists(nofacts,bound,equal_var)){
//			find=true;
//			matches_id.insert({id,rm});
//			return id;
//		}
//	}
//	else{
//
//		pair<bool, index_object> termBoundIndex = createIndex(bound);
//
//		AtomTable* matchingTable=new AtomTable;
//
//		if(termBoundIndex.first){
//			auto pair=factsIndexMap.equal_range(termBoundIndex.second);
//			for(auto it=pair.first;it!=pair.second;it++)
//				matchingTable->insert(it->second);
//		}
//		else
//			matchingTable=facts;
//
//		//Search in facts for match
//		computeFirstMatch(matchingTable,bound,bind,boundFunction,bindFunction,equal_var,rm);
//
//		//If it is EDB and it is not all bound, search also in no facts for match
//		if(!predicate->isEdb() && nofacts->size()>0){
//
//			matchingTable->clear();
//
//			if(termBoundIndex.first){
//				auto pair=nofactsIndexMap.equal_range(termBoundIndex.second);
//				for(auto it=pair.first;it!=pair.second;it++)
//					matchingTable->insert(it->second);
//			}
//			else
//				matchingTable=nofacts;
//
//			computeFirstMatch(matchingTable,bound,bind,boundFunction,bindFunction,equal_var,rm);
//
//		}
//
//	}
//
//	matches_id.insert({id,rm});
//	nextMatch(id,bind,boundFunction,bindFunction,find);
//	return id;
//}


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

