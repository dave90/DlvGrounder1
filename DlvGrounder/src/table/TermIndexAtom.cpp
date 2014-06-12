/*
 * FirstTermIndexAtom.cpp
 *
 *  Created on: 17/mag/2014
 *      Author: Jessica
 */

#include "TermIndexAtom.h"
#include <boost/lexical_cast.hpp>
#include "../utility/Timer.h"

long TermIndexAtom::determineTermToBeIndexed(vec_pair_index_object& bound) {
	//Check if the term of indexing is bound
	int termBoundIndex = -1;
	if(!termSetByPreference){
		termToBeIndexed=bound[0].first;
		termBoundIndex = bound[0].second;
		termSetByPreference=true;
		return termBoundIndex;
	}
	// If it is not possible to indexing with the set term, the indexing is not done at all
	for (unsigned int i = 0; i < bound.size(); i++)
		if (bound[i].first == termToBeIndexed) {
			termBoundIndex = bound[i].second;
			break;
		}
	return termBoundIndex;
}


index_object TermIndexAtom::firstMatch(vec_pair_index_object& bound, vec_pair_index_object& bind, map_int_int& equal_var, bool& find) {

	index_object id = matches_id.size();
	ResultMatch* rm=new ResultMatch(bind);


	long termBoundIndex = determineTermToBeIndexed(bound);
	AtomTable* matchingTable;
	if(termBoundIndex!=-1){
		if(!instantiateIndexMap)
			initializeIndexMap();
		matchingTable=&indexMap[termBoundIndex];
	}
	else
		matchingTable=atoms;

	if(bind.size()==0){
		find=findIfAFactExists(*atoms,bound,equal_var);
		matches_id.insert({id,rm});
		return id;
	}

	computeFirstMatch(*matchingTable,bound,bind,equal_var,rm);

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;

}


index_object TermIndexAtomMultiMap::firstMatch(vec_pair_index_object& bound, vec_pair_index_object& bind, map_int_int& equal_var, bool& find) {

	index_object id = matches_id.size();
	ResultMatch *rm = new ResultMatch(bind);

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
				find = findIfAFactExists(*atoms,bound, equal_var);

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
//	Timer::getInstance()->start("Creation Index Structure");
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
//	Timer::getInstance()->end();
	instantiateIndexMap=true;
}

void TermIndexAtomMultiMap::initializeIndexMap(){
//	Timer::getInstance()->start("Creation Index Structure");
	for (GenericAtom*a : *atoms) {
		index_object termIndex=a->atom->getTerm(termToBeIndexed);
		indexMap.insert({termIndex,a});
	}
//	Timer::getInstance()->end();
	instantiateIndexMap=true;
}
