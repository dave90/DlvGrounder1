/*
 * FirstTermIndexAtom.cpp
 *
 *  Created on: 17/mag/2014
 *      Author: Jessica
 */

#include "TermIndexAtom.h"
#include <boost/lexical_cast.hpp>
#include "../utility/Timer.h"

void TermIndexAtom::determineTermToBeIndexed(vec_pair_index_object& bound) {
	if(!termSetByPreference && bound.size()>0){
		termToBeIndexed = bound[0].first;
		termSetByPreference = true;
	}
}


unsigned int TermIndexAtom::firstMatch(vec_pair_index_object& bound, vec_pair_index_object& bind, map_int_int& equal_var, bool& find) {

	unsigned int id = counter;counter++;
	ResultMatch* rm=new ResultMatch(bind);

	AtomTable* matchingTable;

	if(!termSetByPreference)
		determineTermToBeIndexed(bound);

	pair<bool, index_object>  termBoundIndex({false,0});
	for (unsigned int i = 0; i < bound.size(); i++)
		if (bound[i].first == termToBeIndexed) {
			termBoundIndex.first = true;
			termBoundIndex.second = bound[i].second;
			break;
		}

	if(termBoundIndex.first){
		if(!instantiateIndexMap)
			initializeIndexMap();
		matchingTable=&indexMap[termBoundIndex.second];
	}
	else
		matchingTable=atoms;

	if(bound.size()==(*atoms->begin())->atom->getTermsSize()){
		if(findIfAFactExists(*matchingTable,bound,equal_var)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}
	else
		computeFirstMatch(*matchingTable,bound,bind,equal_var,rm);

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;

}


unsigned int TermIndexAtomMultiMap::firstMatch(vec_pair_index_object& bound, vec_pair_index_object& bind, map_int_int& equal_var, bool& find) {

	unsigned int id = counter;counter++;
	ResultMatch* rm=new ResultMatch(bind);

	if(!termSetByPreference)
		determineTermToBeIndexed(bound);

	pair<bool, index_object>  termBoundIndex({false,0});
	for (unsigned int i = 0; i < bound.size(); i++)
		if (bound[i].first == termToBeIndexed) {
			termBoundIndex.first = true;
			termBoundIndex.second = bound[i].second;
			break;
		}

	if(bound.size()==(*atoms->begin())->atom->getTermsSize()){
		if(findIfAFactExists(*atoms,bound,equal_var)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}
	else{
		AtomTable* matchingTable=new AtomTable;

		if(termBoundIndex.first){
			if(!instantiateIndexMap)
				initializeIndexMap();
			auto pair=indexMap.equal_range(termBoundIndex.second);
			for(auto it=pair.first;it!=pair.second;it++)
				matchingTable->insert(it->second);
		}
		else
			matchingTable=atoms;

		computeFirstMatch(*matchingTable,bound,bind,equal_var,rm);
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;
}

void TermIndexAtomMultiMap::determineTermToBeIndexed(vec_pair_index_object& bound) {
	if(!termSetByPreference && bound.size()>0){
		termToBeIndexed = bound[0].first;
		termSetByPreference = true;
	}
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
