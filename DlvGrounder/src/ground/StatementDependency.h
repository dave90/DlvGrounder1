/*
 * StatementDependency.h
 *
 *  Created on: 03/apr/2014
 *      Author: Davide
 */

#ifndef STATEMENTDEPENDENCY_H_
#define STATEMENTDEPENDENCY_H_

#include <unordered_map>
#include <unordered_set>

#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "../statement/Rule.h"

using namespace std;


class StatementAtomMapping {
public:
	StatementAtomMapping(){};
	void addRule(Rule *r);
	const vector<Rule*> getRuleInHead(unsigned long p);
	const vector<Rule*> getRuleInBody(unsigned long p);
	bool isInHead(unsigned long p);
	virtual ~StatementAtomMapping();
private:
	unordered_multimap<unsigned long, Rule*> headMap;
	unordered_multimap<unsigned long, Rule*> bodyMap;

};


// The vertex of the Dependency graph
struct predicate_vertex {
	// Id of the predicate
    unsigned long pred_id;
};

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS,predicate_vertex ,boost::property<boost::edge_weight_t, int>> Graph;

class StatementDependency {
public:
	StatementDependency();
	// Add the mapping head and body with the rule
	void addRuleMapping(Rule *r);
	// Create Dependency Graph
	void createDependency(vector<Rule*>& rules);
	//Create Component Graph
	void createComponent(vector<Rule*>& rules);
	void printDepGraph();
	void printCompGraph();

	virtual ~StatementDependency();
private:

	void addEdgeInDependencyGraph(unsigned long pred_body,unsigned long pred_head);
	void addEdgeInComponentGraph(unsigned long pred_body,unsigned long pred_head,int weight);

	// Calculate the strong components with boost function
	void calculateStrongComponent();

	/*
	 *  Dependency Graph and Component Graph
	 */
	Graph depGraph,compGraph;
	/*
	 *  Map that contains key = Id of the predicate , value = id of the vertex in depGraph
	 */
	unordered_map<unsigned long, unsigned int> predicateIndexGMap;
	/*
	 * For each predicate (in indices) relative component
	 */
	vector<unsigned int> component;
	/*
	 *  Mapping among atom and statement
	 */
	StatementAtomMapping statementAtomMapping;
};


#endif /* STATEMENTDEPENDENCY_H_ */
