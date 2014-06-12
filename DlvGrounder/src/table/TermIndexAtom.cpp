/*
 * FirstTermIndexAtom.cpp
 *
 *  Created on: 17/mag/2014
 *      Author: Jessica
 */

#include "TermIndexAtom.h"
#include <boost/lexical_cast.hpp>
#include "../utility/Timer.h"

index_object TermIndexAtom::firstMatch(vec_pair_long& bound, vec_pair_long& bind, map_int_int& equal_var, bool& find) {

	index_object id = matches_id.size();
	ResultMatch* rm=new ResultMatch(bind);
	unordered_set<string> result_string;

	//Check if the term of indexing is bound
	long termBoundIndex=-1;
	for (unsigned int i = 0; i < bound.size(); ++i)
		if(bound[i].first==termToBeIndexed){
			termBoundIndex=bound[i].second;
			break;
		}

	//If the term of indexing is bound
	if(termBoundIndex!=-1){ //&& indexMap.count(termBoundIndex) --> Puo' non essere inizializzata

		//Initialize the IndexMap if it is not yet initialized
		if(!instantiateIndexMap)
			initializeIndexMap();

		//If the term of indexing is bound and the bound size is 1, then only this term is bound
		if(bound.size()==1){
			for(GenericAtom* a:indexMap[termBoundIndex]){
				if(bind.size()==0){
					find=true;
					matches_id.insert({id,rm});
					return id;
				}
				rm->result.insert(a->atom);
			}
		}
		//Else check the match among the hash set indexed by that term
		else{
			//If all the variables are bound, just check if there exists a fact of this kind
			if(bind.size()==0){
				find = findIfAFactExists(bound, equal_var);

				matches_id.insert({id,rm});
				return id;
			}
			//Else compute the matching facts among those of the hash set indexed by that term
			else if(computeFirstMatch(indexMap[termBoundIndex],bound,bind,equal_var,rm)){
				find=true;
				matches_id.insert({id,rm});
				return id;
			}
		}
	}
	else{
		//If the given term is not bound, just iterate among all facts, like in SimpleIndexAtom
		if(computeFirstMatch(*atoms,bound,bind,equal_var,rm)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;

}


bool TermIndexAtom::findIfAFactExists(vec_pair_long& bound, map_int_int& equal_var) {
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

	bool find = indexMap[predicate].count(genAtom);

	delete genAtom;
	delete atom;

	return find;
}

index_object TermIndexAtomMultiMap::firstMatch(vec_pair_long& bound, vec_pair_long& bind, map_int_int& equal_var, bool& find) {

	index_object id = matches_id.size();
	ResultMatch *rm = new ResultMatch(bind);
	unordered_set<string> result_string;

	//Check if the term of indexing is bound
	long termBoundIndex=-1;
	for (unsigned int i = 0; i < bound.size(); ++i)
		if(bound[i].first==termToBeIndexed){
			termBoundIndex=bound[i].second;
			break;
		}

	//If the term of indexing is bound
	if(termBoundIndex!=-1){ //&& indexMap.count(termBoundIndex) --> Puo' non essere inizializzata

		//Initialize the IndexMap if it is not yet initialized
		if(!instantiateIndexMap)
			initializeIndexMap();

		//If the term of indexing is bound and the bound size is 1, then only this term is bound
		if(bound.size()==1){
			auto pair=indexMap.equal_range(termBoundIndex);
			for(auto it=pair.first;it!=pair.second;it++){
				if(bind.size()==0){
					find=true;
					matches_id.insert({id,rm});
					return id;
				}
				rm->result.insert(it->second->atom);
			}
		}
		//Else check the match among the hash set indexed by that term
		else{
			if(bind.size()==0){
				find = findIfAFactExists(bound, equal_var);

				matches_id.insert({id,rm});
				return id;
			}
			//Else compute the matching facts among those of the hash set indexed by that term
			AtomTable atomMatching;
			auto pair=indexMap.equal_range(termBoundIndex);
			for(auto it=pair.first;it!=pair.second;it++)
				atomMatching.insert(it->second);
			if(computeFirstMatch(atomMatching,bound,bind,equal_var,rm)){
				find=true;
				matches_id.insert({id,rm});
				return id;
			}
		}
	}

	else{
		//If the given term is not bound, just iterate among all facts, like in SimpleIndexAtom
		if(computeFirstMatch(*atoms,bound,bind,equal_var,rm)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;
}


void TermIndexAtom::initializeIndexMap(){
	Timer::getInstance()->start("Creation Index Structure");
	unordered_set<index_object> termToBeIndexedIndices;

	for (GenericAtom*a : *atoms) {
		index_object termIndex=a->atom->getTerm(termToBeIndexed);
		if(!termToBeIndexedIndices.count(termIndex)){
			termToBeIndexedIndices.insert(termIndex);
			AtomTable values;
			values.insert(a);
			indexMap.insert({termIndex,values});
		}
		else{
			indexMap[termIndex].insert(a);
		}
	}
	Timer::getInstance()->end();
	instantiateIndexMap=true;
}

void TermIndexAtomMultiMap::initializeIndexMap(){
	Timer::getInstance()->start("Creation Index Structure");
	for (GenericAtom*a : *atoms) {
		index_object termIndex=a->atom->getTerm(termToBeIndexed);
		indexMap.insert({termIndex,a});
	}
	Timer::getInstance()->end();
	instantiateIndexMap=true;
}

bool TermIndexAtomMultiMap::findIfAFactExists(vec_pair_long& bound, map_int_int& equal_var) {
//	vector<index_object> terms(bound.size() + equal_var.size());
//	for (unsigned int i = 0; i < bound.size(); i++) {
//		terms[bound[i].first] = bound[i].second;
//	}
//	for (auto it : equal_var) {
//		terms[it.second] = terms[it.first];
//	}
	index_object predicate=(*atoms->begin())->atom->getPredicate().second;
//	Atom *atom=new ClassicalLiteral(predicate,terms,false,false);
//	GenericAtom *genAtom=new GenericAtom;
//	genAtom->atom=atom;

	bool find = indexMap.count(predicate);

//	delete genAtom;
//	delete atom;

	return find;
}
