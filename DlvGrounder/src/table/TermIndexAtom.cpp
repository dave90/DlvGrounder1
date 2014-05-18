/*
 * FirstTermIndexAtom.cpp
 *
 *  Created on: 17/mag/2014
 *      Author: Jessica
 */

#include "TermIndexAtom.h"
#include <boost/lexical_cast.hpp>

unsigned long TermIndexAtom::firstMatch(vec_pair_long& bound, vec_pair_long& bind, map_int_int& equal_var, bool& find) {

	if(!instantiateIndexMap)
		inizializeIndexMap();

	unsigned long id = matches_id.size();
	ResultMatch *rm = new ResultMatch;
	unordered_set<string> result_string;

	//Check if first term matches
	if(bound.size()>termToBeIndexed && bound[termToBeIndexed].second!=0 && indexMap.count(bound[termToBeIndexed].second)){

		//Check if only first term is bound
		bool onlyFirstTermBound=true;
		if(bound.size()>termToBeIndexed && bound[termToBeIndexed].second!=0){
			for (unsigned int i = 1; i < bound.size(); i++)
				if(bound[i].second!=0){
					onlyFirstTermBound=!onlyFirstTermBound;
					break;
				}
		}

		AtomTable possibleMatchingAtoms=indexMap.find(bound[termToBeIndexed].second)->second;

		if(onlyFirstTermBound){
			for(auto it=possibleMatchingAtoms.begin();it!=possibleMatchingAtoms.end();it++)
				rm->result.push_back(*it);
		}

		else{
			if(computeFirstMatch(possibleMatchingAtoms,bound,bind,equal_var,find,id,rm,result_string))
					return id;
			matches_id.insert({id,rm});
			nextMatch(id,bind,find);
			return id;
		}

	}
	else{
		for(auto it=indexMap.begin();it!=indexMap.end();it++){
			if(computeFirstMatch(it->second,bound,bind,equal_var,find,id,rm,result_string))
					return id;
			matches_id.insert({id,rm});
			nextMatch(id,bind,find);
			return id;
		}
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;

}


void TermIndexAtom::inizializeIndexMap(){
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
			auto it=indexMap.find(termIndex);
			(it)->second.insert(a);
		}
	}
	instantiateIndexMap=true;

//		for(auto it : indexMap){
//			for(Atom* a : it.second){
//				cout<<IdsManager::getString(IdsManager::ATOM_ID_MANAGER,a->getIndex())<<endl;
//				for(unsigned int t : a->getTerms())
//					cout<<IdsManager::getString(IdsManager::PREDICATE_ID_MANAGER,t);
//			}
//			cout<<endl;
//		}
}

