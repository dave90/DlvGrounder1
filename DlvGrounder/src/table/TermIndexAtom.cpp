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
			for (unsigned int i = 0; i < bound.size(); i++)
				if(i!=termToBeIndexed && bound[i].second!=0){
					onlyFirstTermBound=false;
					break;
				}
		}

		AtomTable* possibleMatchingAtoms=indexMap[bound[termToBeIndexed].second];

		if(onlyFirstTermBound){
			for(auto it=possibleMatchingAtoms->begin();it!=possibleMatchingAtoms->end();it++)
				rm->result.push_back(*it);
		}

		else{
			if(computeFirstMatch(possibleMatchingAtoms,bound,bind,equal_var,find,id,rm,result_string))
					return id;
		}

	}
	else{
		for(auto it=indexMap.begin();it!=indexMap.end();it++){
			if(computeFirstMatch(it->second,bound,bind,equal_var,find,id,rm,result_string))
					return id;
		}
	}

	matches_id.insert({id,rm});
	nextMatch(id,bind,find);
	return id;

}

TermIndexAtom::~TermIndexAtom(){
	for(auto it=indexMap.begin();it!=indexMap.end();it++)
		delete(it->second);
};


void TermIndexAtom::inizializeIndexMap(){
	unordered_set<unsigned long> termToBeIndexedIndices;
	for (Atom *a : *atoms) {
		unsigned long termIndex=a->getTerm(termToBeIndexed);
		if(!termToBeIndexedIndices.count(termIndex)){
			termToBeIndexedIndices.insert(termIndex);
			AtomTable* values=new AtomTable;
			values->insert(a);
			indexMap.insert({termIndex,values});
		}
		else{
			indexMap[termIndex]->insert(a);
		}
	}
	instantiateIndexMap=true;
//	cout<<"iniz"<<endl;
//		for(auto it : indexMap){
//			for(Atom* a : it.second){
//				cout<<IdsManager::getString(IdsManager::PREDICATE_ID_MANAGER,a->getPredicate())<<endl;
//				for(unsigned int t : a->getTerms())
//					cout<<IdsManager::getString(IdsManager::TERM_ID_MANAGER,t);
//			}
//			cout<<endl;
//		}
}

