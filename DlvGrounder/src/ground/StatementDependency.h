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


typedef boost::adjacency_list< boost::setS, boost::vecS , boost::directedS,predicate_vertex> Graph;
typedef boost::adjacency_list< boost::setS, boost::vecS , boost::directedS ,predicate_vertex,boost::property<boost::edge_weight_t, int>> WeightGraph;

class DependencyGraph{
public:
	DependencyGraph(){};
	// Create Dependency Graph
	void createDependency(vector<Rule*>& rules,StatementAtomMapping &statementAtomMapping);
	// Calculate strong component and put predicate_id and component
	void calculateStrongComponent(unordered_map<unsigned long,unsigned int> &component);

	void addEdge(unsigned long pred_body,unsigned long pred_head);
	void deleteVertex(unsigned long pred);

	// Print predicate index
	void print();

private:
	/*
	 *  Dependency Graph and Component Graph
	 */
	Graph depGraph;
	/*
	 *  Map that contains key = Id of the predicate , value = id of the vertex in depGraph
	 */
	unordered_map<unsigned long, unsigned int> predicateIndexGMap;
};

class ComponentGraph{
public:
	ComponentGraph(){};
	// Create Dependency Graph
	void createComponent(vector<Rule*>& rules,DependencyGraph &depGraph,StatementAtomMapping &statementAtomMapping);


	void addEdge(unsigned long pred_body,unsigned long pred_head,int weight);
	void print();

private:
	// Calculate the strong components with boost function

	/*
	 *  Dependency Graph and Component Graph
	 */
	WeightGraph compGraph;
	/*
	 * For each predicate (in indices) relative component
	 */
	unordered_map<unsigned long,unsigned int> component;
};




class StatementDependency {
public:
	StatementDependency(){};
	// Add the mapping head and body with the rule
	void addRuleMapping(Rule *r);
	void createDependencyGraph();
	void createComponentGraph();
	void createDependencyComponentGraph();

	void print();

	virtual ~StatementDependency();
private:
	DependencyGraph depGraph;
	ComponentGraph compGraph;
	StatementAtomMapping statementAtomMapping;
	vector<Rule*> rules;
};


#endif /* STATEMENTDEPENDENCY_H_ */
