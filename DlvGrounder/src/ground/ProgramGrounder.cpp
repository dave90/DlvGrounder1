/*
 * ProgramGrounder.cpp
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#include "ProgramGrounder.h"

void ProgramGrounder::ground() {
	statementDependency->createDependencyGraph(predicateTable);
	statementDependency->createComponentGraph();

	//Ground first rule
	groundRule(statementDependency->getRule(0));
}

void ProgramGrounder::groundRule(Rule* r) {

//ONLY FIRST NO ANONIMUS
	//variable,value
	unordered_map<unsigned long,unsigned long> bound;

	for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {

		Atom *atom=*body_it;
		vector<unsigned long> bounds_,bind_,values_,result_;
		//FOR each term calculate bound & bind
		for(int i=0;i<predicateTable->getPredicate(atom->getPredicate())->getArity();i++){
			//calculate bound
			auto it=bound.find(atom->getTerms()[i]);
			if(it!=bound.end()){
				bounds_.push_back(i);
				values_.push_back(it->second);
			}else{
				bind_.push_back(i);
			}
		}
		instancesTable->getInstance(atom->getPredicate())->getIndex()->firstMatch(bounds_,values_,bind_,result_);
		for(int i=0;i<result_.size();i++){
			bound.insert({bind_[i],result_[i]});
		}

	}

	for(auto it:bound){
		cout<<IdsManager::getStringStrip(IdsManager::TERM_ID_MANAGER,it.first)<<"="<<IdsManager::getStringStrip(IdsManager::TERM_ID_MANAGER,it.second)<<"  ";
	}
	cout<<endl;

}

ProgramGrounder::~ProgramGrounder() {
	delete predicateTable;
	delete instancesTable;
	delete statementDependency;
	delete termsMap;

}

