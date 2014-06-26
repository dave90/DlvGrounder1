/*
 * Rule.cpp
 *
 *  Created on: 03/apr/2014
 *      Author: Jessica
 */

#include "Rule.h"
#include "iostream"

void  Rule::print(){
	unsigned int i=0;
	for (auto atom:head) {
		atom->print();
		if(i!=head.size()-1)
			cout<<";";
		i++;
	}
	if(body.size()>0){
		cout<<":-";
		unsigned int i=0;
		for (auto atom:body) {
			atom->print();
			if(i!=head.size()-1)
				cout<<";";
			i++;
		}
	}
	cout<<"."<<endl;
}

unordered_set<index_object> Rule::getPredicateInHead() {
	unordered_set<index_object> predicates;
	for (auto atom:head) {
		pair<bool,index_object> predicate=atom->getPredicate();
		if(predicate.first)
			predicates.insert(predicate.second); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<index_object> Rule::getPredicateInBody() {
	unordered_set<index_object> predicates;
	for (auto atom:body) {
		pair<bool,index_object> predicate=atom->getPredicate();
		if(predicate.first)
			predicates.insert(predicate.second); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<index_object> Rule::getPositivePredicateInBody() {
	unordered_set<index_object> predicates;
	for (auto atom:head) {
		pair<bool,index_object> predicate=atom->getPredicate();
		if(predicate.first && !atom->isNegative())
			predicates.insert(predicate.second); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<index_object> Rule::getNegativePredicateInBody() {
	unordered_set<index_object> predicates;
	for (auto atom:body) {
		pair<bool,index_object> predicate=atom->getPredicate();
		if(predicate.first && atom->isNegative())
			predicates.insert(predicate.second); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

bool Rule::operator ==(const Rule& r) {
	  if(getSizeBody()!=r.getSizeBody())return false;
	  if(getSizeHead()!=r.getSizeHead())return false;
	  for(auto atom:head){
		  bool find=false;
		  for(auto it=r.getBeginHead();it!=r.getEndHead();it++)
			  if(*atom==*(*it)){
				  find=true;
				  break;
			  }
		  if(!find)return false;
	  }
	  for(auto atom:body){
		  bool find=false;
		  for(auto it=r.getBeginBody();it!=r.getEndBody();it++)
			  if(*atom==*(*it)){
				  find=true;
				  break;
			  }
		  if(!find)return false;
	  }

	  return true;

}

Rule::~Rule() {
	for(auto atom:head)delete atom;
	for(auto atom:body)delete atom;
}
