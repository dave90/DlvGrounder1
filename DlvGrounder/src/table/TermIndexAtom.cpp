/*
 * FirstTermIndexAtom.cpp
 *
 *  Created on: 17/mag/2014
 *      Author: Jessica
 */

#include "TermIndexAtom.h"
#include <boost/lexical_cast.hpp>

unsigned long TermIndexAtom::firstMatch(vec_pair_long& bound, vec_pair_long& bind, map_int_int& equal_var, bool& find) {

	unsigned long id = matches_id.size();
	ResultMatch *rm = new ResultMatch;
	unordered_set<string> result_string;

	pair<int,long> termBoundIndex ( -1, -1);
	for (unsigned int i = 0; i < bound.size(); ++i)
		if(bound[i].first==termToBeIndexed){
			termBoundIndex=bound[i];
			break;
		}

	//Check if the given term is bound
	if(termBoundIndex.second!=-1){

		//Initialize the IndexMap only if it is needed, just at beginning
		if(!instantiateIndexMap)
			initializeIndexMap();

		//Check if only the given term is bound
		//If only the given term is bound, return the hash set indexed by that term
		if(bound.size()!=1){ // && indexMap.count(bound[termToBeIndexed].second
			for(Atom* a:indexMap[termBoundIndex.second]){
				if(bind.size()==0){
					find=true;
					matches_id.insert({id,rm});
					return id;
				}
				rm->result.insert(a);
			}
		}
		//Else check the match among the hash set indexed by that term
		else
			if(computeFirstMatch(indexMap[termBoundIndex.second],bound,bind,equal_var,rm,result_string)){
				find=true;
				matches_id.insert({id,rm});
				return id;
			}

	}
	else{
		//If the given term is not bound, just iterate among all facts, like in SimpleIndexAtom
		if(computeFirstMatch(*atoms,bound,bind,equal_var,rm,result_string)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;

}


unsigned long TermIndexAtomMultiMap::firstMatch(vec_pair_long& bound, vec_pair_long& bind, map_int_int& equal_var, bool& find) {

	unsigned long id = matches_id.size();
	ResultMatch *rm = new ResultMatch;
	unordered_set<string> result_string;

	pair<int,long> termBoundIndex ( -1, -1);
	for (unsigned int i = 0; i < bound.size(); ++i)
		if(bound[i].first==termToBeIndexed){
			termBoundIndex.first=bound[i].first;
			termBoundIndex.second=bound[i].second;
			break;
		}

	//Check if the given term is bound
	if(termBoundIndex.second!=-1){

		//Initialize the IndexMap only if it is needed
		if(!instantiateIndexMap)
			initializeIndexMap();

		//Check if only the given term is bound
		//If only the given term is bound, return the hash set indexed by that term
		if(bound.size()!=1){ // && indexMap.count(bound[termToBeIndexed].second
			auto pair=indexMap.equal_range(termBoundIndex.second);
			for(auto it=pair.first;it!=pair.second;it++){
				if(bind.size()==0){
					find=true;
					matches_id.insert({id,rm});
					return id;
				}
				rm->result.insert(it->second);
			}
		}
		//Else check the match among the hash set indexed by that term
		else{
			AtomTable atomMatching;
			auto pair=indexMap.equal_range(termBoundIndex.second);
			for(auto it=pair.first;it!=pair.second;it++)
				atomMatching.insert(it->second);
			if(computeFirstMatch(atomMatching,bound,bind,equal_var,rm,result_string)){
				find=true;
				matches_id.insert({id,rm});
				return id;
			}
		}
	}
	else{
		//If the given term is not bound, just iterate among all facts, like in SimpleIndexAtom
		if(computeFirstMatch(*atoms,bound,bind,equal_var,rm,result_string)){
			find=true;
			matches_id.insert({id,rm});
			return id;
		}
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;

}

TermIndexAtom::~TermIndexAtom(){
//	for(auto it=indexMap.begin();it!=indexMap.end();it++)
//		delete(it->second);
};


void TermIndexAtom::initializeIndexMap(){
	unordered_set<unsigned long> termToBeIndexedIndices;
	for (Atom *a : *atoms) {
		unsigned long termIndex=a->getTerm(termToBeIndexed);
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
	instantiateIndexMap=true;
//	cout<<"iniz"<<endl;
//		for(auto it : indexMap){
//			for(Atom* a : it.second){
//				cout<<IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,a->getPredicate())<<endl;
//				for(unsigned int t : a->getTerms())
//					cout<<IdsManager::getString(IdsManager::TERM_ID_MANAGER,t);
//			}
//			cout<<endl;
//		}
}

void TermIndexAtomMultiMap::initializeIndexMap(){
	for (Atom *a : *atoms) {
		unsigned long termIndex=a->getTerm(termToBeIndexed);
		indexMap.insert({termIndex,a});
	}
	instantiateIndexMap=true;
}

