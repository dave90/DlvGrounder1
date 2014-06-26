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

pair<bool, index_object> TermIndexAtom::findTermBoundIndex(vec_pair_index_object& bound) {
	if (!termSetByPreference)
		determineTermToBeIndexed(bound);

	pair<bool, index_object> termBoundIndex( { false, 0 });
	for (unsigned int i = 0; i < bound.size(); i++)
		if (bound[i].first == termToBeIndexed) {
			termBoundIndex.first = true;
			termBoundIndex.second = bound[i].second;
			if (!instantiateIndexMap)
				initializeIndexMap();

			break;
		}
	return termBoundIndex;
}

pair<bool, index_object> TermIndexAtomMultiMap::findTermBoundIndex(vec_pair_index_object& bound) {
	if (!termSetByPreference)
		determineTermToBeIndexed(bound);

	pair<bool, index_object> termBoundIndex( { false, 0 });
	for (unsigned int i = 0; i < bound.size(); i++)
		if (bound[i].first == termToBeIndexed) {
			termBoundIndex.first = true;
			termBoundIndex.second = bound[i].second;
			if (!instantiateIndexMap)
				initializeIndexMap();

			break;
		}
	return termBoundIndex;
}

unsigned int TermIndexAtom::firstMatch(vec_pair_index_object& bound, vec_pair_index_object& bind, map_int_int& equal_var, bool& find) {

	unsigned int id = counter;counter++;
	ResultMatch* rm=new ResultMatch(bind);

	AtomTable* matchingTable;

	pair<bool, index_object> termBoundIndex = findTermBoundIndex(bound);

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

	//If it is EDB and it is not all bound, search also in nofacts for match
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


unsigned int TermIndexAtomMultiMap::firstMatch(vec_pair_index_object& bound, vec_pair_index_object& bind, map_int_int& equal_var, bool& find) {

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

		pair<bool, index_object> termBoundIndex = findTermBoundIndex(bound);

		AtomTable* matchingTable=new AtomTable;

		if(termBoundIndex.first){
			auto pair=factsIndexMap.equal_range(termBoundIndex.second);
			for(auto it=pair.first;it!=pair.second;it++)
				matchingTable->insert(it->second);
		}
		else
			matchingTable=facts;

		computeFirstMatch(matchingTable,bound,bind,equal_var,rm);

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

void TermIndexAtomMultiMap::determineTermToBeIndexed(vec_pair_index_object& bound) {
	if(!termSetByPreference && bound.size()>0){
		termToBeIndexed = bound[0].first;
		termSetByPreference = true;
	}
}


void TermIndexAtom::initializeIndexMap(){
//	Timer::getInstance()->start("Creation Index Structure");
	unordered_set<index_object> termToBeIndexedIndices;

	for (GenericAtom*a : *facts) {
		index_object termIndex=a->terms[termToBeIndexed];
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
			index_object termIndex=a->terms[termToBeIndexed];
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
	instantiateIndexMap=true;
}

void TermIndexAtomMultiMap::initializeIndexMap(){
//	Timer::getInstance()->start("Creation Index Structure");
	for (GenericAtom*a : *facts) {
		index_object termIndex=a->terms[termToBeIndexed];
		factsIndexMap.insert({termIndex,a});
	}
	if(!predicate->isEdb()){
		for (GenericAtom*a : *nofacts) {
				index_object termIndex=a->terms[termToBeIndexed];
				nofactsIndexMap.insert({termIndex,a});
		}
	}
//	Timer::getInstance()->end();
	instantiateIndexMap=true;
}
