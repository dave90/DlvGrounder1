/*
 * Rule.cpp
 *
 *  Created on: 03/apr/2014
 *      Author: Jessica
 */

#include "Rule.h"
#include "iostream"

void  Rule::print(TermTable* tb){
	for (unsigned int i = 0; i < head.size(); ++i) {
		head[i]->print(tb);
		if(i!=head.size()-1)
			cout<<" | ";
	}
	if(body.size()>0){
		cout<<" :- ";
		for (unsigned int i = 0; i < body.size(); ++i) {
			body[i]->print(tb);
			if(i!=body.size()-1)
				cout<<",";
		}
	}
	cout<<".";
}

unordered_set<index_object> Rule::getPredicateInHead() {
	unordered_set<index_object> predicates;
	for (unsigned int i = 0; i < head.size(); ++i) {
		pair<bool,index_object> predicate=head[i]->getPredicate();
		if(predicate.first)
			predicates.insert(predicate.second); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<index_object> Rule::getPredicateInBody() {
	unordered_set<index_object> predicates;
	for (unsigned int i = 0; i < body.size(); ++i) {
		pair<bool,index_object> predicate=body[i]->getPredicate();
		if(predicate.first)
			predicates.insert(predicate.second); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<index_object> Rule::getPositivePredicateInBody() {
	unordered_set<index_object> predicates;
	for (unsigned int i = 0; i < body.size(); ++i) {
		pair<bool,index_object> predicate=body[i]->getPredicate();
		if(predicate.first && !body[i]->isNegative())
			predicates.insert(predicate.second); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<index_object> Rule::getNegativePredicateInBody() {
	unordered_set<index_object> predicates;
	for (unsigned int i = 0; i < body.size(); ++i) {
		pair<bool,index_object> predicate=body[i]->getPredicate();
		if(predicate.first && body[i]->isNegative())
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
