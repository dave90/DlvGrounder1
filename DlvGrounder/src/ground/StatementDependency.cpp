/*
 * StatementDependency.cpp
 *
 *  Created on: 03/apr/2014
 *      Author: Davide
 */

#include "StatementDependency.h"

#include <boost/graph/graph_utility.hpp>

StatementDependency::StatementDependency() {

}

void StatementDependency::addRule(Rule* r) {
	unordered_set<unsigned long> head = r->getPredicateInHead();
	unordered_set<unsigned long> body = r->getPositivePredicateInBody();
	for (unsigned long i : body)
		for (unsigned long j : head)
			boost::add_edge(i, j, depGraph);
}

StatementDependency::~StatementDependency() {
	// TODO Auto-generated destructor stub
}

void StatementAtomMapping::addRule(Rule* r) {
	unordered_set<unsigned long> head = r->getPredicateInHead();
	unordered_set<unsigned long> body = r->getPredicateInBody();
	for (unsigned long i : head) {
		headMap.insert( { i, r });
	}
	for (unsigned long i : body) {
		headMap.insert( { i, r });
	}
}

const vector<Rule*> StatementAtomMapping::getRuleInHead(unsigned long p) {
	vector<Rule*> rules;
	auto pair1 = headMap.equal_range(p);
	for (; pair1.first != pair1.second; ++pair1.first)
		rules.push_back(pair1.first->second);
	return rules;
}

const vector<Rule*> StatementAtomMapping::getRuleInBody(unsigned long p) {
	vector<Rule*> rules;
	auto pair1 = bodyMap.equal_range(p);
	for (; pair1.first != pair1.second; ++pair1.first)
		rules.push_back(pair1.first->second);
	return rules;
}

void StatementDependency::printDepGraph() {
    boost::print_graph(depGraph);
}
