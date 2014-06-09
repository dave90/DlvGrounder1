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
	cout<<" :- ";
	for (unsigned int i = 0; i < body.size(); ++i) {
		body[i]->print(tb);
		if(i!=body.size()-1)
			cout<<",";
	}
	cout<<"."<<endl;
}

unordered_set<index_object> Rule::getPredicateInHead() {
	unordered_set<index_object> predicates;
	for (unsigned int i = 0; i < head.size(); ++i) {
		if(head[i]->getPredicate()!=-1)
			predicates.insert(head[i]->getPredicate()); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<index_object> Rule::getPredicateInBody() {
	unordered_set<index_object> predicates;
	for (unsigned int i = 0; i < body.size(); ++i) {
		if(body[i]->getPredicate()!=-1)
			predicates.insert(body[i]->getPredicate()); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<index_object> Rule::getPositivePredicateInBody() {
	unordered_set<index_object> predicates;
	for (unsigned int i = 0; i < body.size(); ++i) {
		if(body[i]->getPredicate()!=-1 && !body[i]->isNegative())
			predicates.insert(body[i]->getPredicate()); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<index_object> Rule::getNegativePredicateInBody() {
	unordered_set<index_object> predicates;
	for (unsigned int i = 0; i < body.size(); ++i) {
		if(body[i]->getPredicate()!=-1 && body[i]->isNegative())
			predicates.insert(body[i]->getPredicate()); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

