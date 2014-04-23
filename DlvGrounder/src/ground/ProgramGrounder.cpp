/*
 * ProgramGrounder.cpp
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#include "ProgramGrounder.h"

#include <list>

void ProgramGrounder::ground() {
	statementDependency->createDependencyGraph(predicateTable);
	statementDependency->createComponentGraph();

	//Ground first rule
	groundRule(statementDependency->getRule(0));
}

void ProgramGrounder::groundRule(Rule* r) {

	//variable,value
	unordered_map<unsigned long, unsigned long> bound;
	list<unsigned long> id_match;

	Atom *a=*r->getBeginBody();
	IndexAtom *index=instancesTable->getInstance(a->getPredicate())->getIndex();

	vec_pair_long bound_,bind_;
	bound_.push_back({0,IdsManager::getIndex(IdsManager::TERM_ID_MANAGER,"1").first});
	bound_.push_back({1,IdsManager::getIndex(IdsManager::TERM_ID_MANAGER,"2").first});
	bound_.push_back({2,IdsManager::getIndex(IdsManager::TERM_ID_MANAGER,"3").first});


	unsigned long id=index->firstMatch(bound_,bind_);
	for(auto t:bind_)
		cout<<t.first<<" "<<IdsManager::getStringStrip(IdsManager::TERM_ID_MANAGER,t.second);
	cout<<endl;
	bind_.clear();
	index->nextMatch(id,bind_);
	for(auto t:bind_)
		cout<<t.first<<" "<<IdsManager::getStringStrip(IdsManager::TERM_ID_MANAGER,t.second);
	cout<<endl;
	bind_.clear();


}

ProgramGrounder::~ProgramGrounder() {
	delete predicateTable;
	delete instancesTable;
	delete statementDependency;
	delete termsMap;

}

