/*
 * StatementDependency.h
 *
 *  Created on: 03/apr/2014
 *      Author: Davide
 */

#ifndef STATEMENTDEPENDENCY_H_
#define STATEMENTDEPENDENCY_H_

#include <unordered_map>

#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "../statement/Rule.h"

using namespace std;


struct Component {
	vector<Rule*> rules;
};

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS > Graph;

class StatementDependency {
public:
	StatementDependency();
	void addRule(Rule *r);
	void addRules(vector<Rule*> rules){for(Rule *r:rules)addRule(r);};
	const vector<Component>& getOrderedComponents();
	void printDepGraph();

	virtual ~StatementDependency();
private:
	Graph depGraph;
};

struct hashAtomRule {
	size_t operator()(unsigned long p) const {
		return p;
	}
	bool operator()(unsigned long p1, unsigned long p2) const {
		return p1==p2;
	}
};

class StatementAtomMapping {
public:
	StatementAtomMapping(){};
	void addRule(Rule *r);
	const vector<Rule*> getRuleInHead(unsigned long p);
	const vector<Rule*> getRuleInBody(unsigned long p);
	virtual ~StatementAtomMapping();
private:
	unordered_multimap<unsigned long, Rule*, hashAtomRule,hashAtomRule> headMap;
	unordered_multimap<unsigned long, Rule*, hashAtomRule,hashAtomRule> bodyMap;

};

#endif /* STATEMENTDEPENDENCY_H_ */
