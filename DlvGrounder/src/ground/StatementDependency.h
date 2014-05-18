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
#include "../table/PredicateTable.h"


using namespace std;

/**
 *  StatementAtomMapping contains the ralation of atoms and statements
 *
 */
class StatementAtomMapping {
public:
	StatementAtomMapping(){};
	/// Add rule in the relation atom statement
	void addRule(Rule *r);
	/// Get rule where the predicate compare in head
	/// @param p is the index of predicate
	/// @param rules the rules that p compare in head
	void getRuleInHead(unsigned long p,vector<Rule*> & rules);
	/// Get rule where the predicate compare in body
	/// @param p is the index of predicate
	const vector<Rule*> getRuleInBody(unsigned long p);
	/// Return true if compare in the head of one rule
	/// @param p is the index of predicate
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

/*
 *  The dependency graph of the program
 */
class DependencyGraph{
public:
	DependencyGraph(){};
	/// Add the atoms in rule in Dependency Graph
	///@param rule of the program
	///@param statementAtomMapping the relation atom statement
	void addInDependency(Rule* rule);
	/// Calculate strong component and put predicate_id and component
	/// The method put for each predicate the relative component in the map component
	void calculateStrongComponent(unordered_map<unsigned long,unsigned int> &component);

	/// Add edge in the graph
	void addEdge(unsigned long pred_body,unsigned long pred_head);
	/// Delete the vertex with predicate in hash set
	/// predicateIndexGMap CHANGED can't use anymore
	void deleteVertex(unordered_set<unsigned long>& delete_pred);

	/// Print predicate index
	void printFile(string fileGraph);
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

/*
 *  The component graph of the program
 */
class ComponentGraph{
public:
	ComponentGraph(){};
	/// Create component graph
	///@param rules is the rules of the program
	///@param statementAtomMapping the relation atom statement
	void createComponent(DependencyGraph &depGraph,StatementAtomMapping &statementAtomMapping);

	/// Add edge in the graph
	void addEdge(unsigned long pred_body,unsigned long pred_head,int weight);
	void computeAnOrdering(vector<unsigned int>& componentsOrdering);
	void computeAllPossibleOrdering(vector<vector<unsigned int>>& componentsOrderings);
	void printFile(string fileGraph);
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

/**
 * 	StatementDependency manage the creation of the dependency graph
 * 	and the component graph
 */
class StatementDependency {
public:
	StatementDependency(){};
	/// Add the mapping head and body with the rule and the graphs are created according to these
	void addRuleMapping(Rule *r);
	void createDependencyGraph(PredicateTable* pt);
	void createComponentGraph();
	Rule* getRule(int i){return rules[i];};

	void print(TermTable *tb);

	virtual ~StatementDependency(){for(Rule *r:rules)delete r;};
private:
	DependencyGraph depGraph;
	ComponentGraph compGraph;
	StatementAtomMapping statementAtomMapping;
	vector<Rule*> rules;
};


#endif /* STATEMENTDEPENDENCY_H_ */
