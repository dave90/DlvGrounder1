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

unordered_set<unsigned long> Rule::getPredicateInHead() {
	unordered_set<unsigned long> predicates;
	for (unsigned int i = 0; i < head.size(); ++i) {
		if(head[i]->getPredicate()!=-1)
			predicates.insert(head[i]->getPredicate()); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<unsigned long> Rule::getPredicateInBody() {
	unordered_set<unsigned long> predicates;
	for (int i = 0; i < body.size(); ++i) {
		if(body[i]->getPredicate()!=-1)
			predicates.insert(body[i]->getPredicate()); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<unsigned long> Rule::getPositivePredicateInBody() {
	unordered_set<unsigned long> predicates;
	for (int i = 0; i < body.size(); ++i) {
		if(body[i]->getPredicate()!=-1 && !body[i]->isNegative())
			predicates.insert(body[i]->getPredicate()); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

unordered_set<unsigned long> Rule::getNegativePredicateInBody() {
	unordered_set<unsigned long> predicates;
	for (int i = 0; i < body.size(); ++i) {
		if(body[i]->getPredicate()!=-1 && body[i]->isNegative())
			predicates.insert(body[i]->getPredicate()); //TODO Con gli aggregati/choice??
	}
	return predicates;
}

