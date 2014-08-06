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
#include <list>

#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "../statement/Rule.h"
#include "../table/PredicateTable.h"


using namespace std;

/**
 *  This class contains the dependency correlation between predicates and statements
 */
class StatementAtomMapping {
public:
	StatementAtomMapping(){};
	///This method adds a rule, determining which predicates appear in it
	void addRule(Rule *r);
	/// Getter for the rule in which the predicate appear in the head
	/// @param p The index of predicate
	/// @param rules The vector that will be filled in with the rules in which p appear in the head
	void getRuleInHead(index_object p,vector<Rule*> & rules);
	/// Getter for the rule in which the predicate appear in the body
	/// @param p The index of predicate
	/// @param rules The vector that will be filled in with the rules in which p appear in the body
	const vector<Rule*> getRuleInBody(index_object p);
	/// Return true if the predicate p appear in the head of at least one rule
	/// @param p The index of predicate
	bool isInHead(index_object p);
	virtual ~StatementAtomMapping();
private:
	//The unordered multimap containing the maps between predicates and rules in which they appear in the head
	unordered_multimap<index_object, Rule*> headMap;
	//The unordered multimap containing the maps between predicates and rules in which they appear in the body
	unordered_multimap<index_object, Rule*> bodyMap;

};


// This utility struct defines a predicate as vertex, containing its index
struct predicate_vertex {
    index_object pred_id;
};

//Definition of a non-weighted graph with adjacency list
typedef boost::adjacency_list< boost::setS, boost::vecS, boost::directedS, predicate_vertex> Graph;
//Definition of a weighted graph with adjacency list
typedef boost::adjacency_list< boost::setS, boost::vecS, boost::directedS, predicate_vertex, boost::property<boost::edge_weight_t, int>> WeightGraph;

/*
 *  This class represents the dependency graph of the program
 */
class DependencyGraph{
public:
	DependencyGraph(){};

	/// This method adds the atoms in the rule in the dependency graph
	void addInDependency(Rule* rule);

	/// This method computes the strong components of the dependency graph.
	/// It also put for each predicate the relative component in the map of components.
	void calculateStrongComponent(unordered_map<index_object,unsigned int> &component);

	/// This method adds an edge in the dependency graph between the two predicate given
	void addEdge(index_object pred_body, index_object pred_head);

	// This method deletes the vertex corresponding to the given predicate
	 void deleteVertex(unordered_set<index_object>& delete_pred);

	/// This method prints the dependency graph in a file using DOT standard format
	void printFile(string fileGraph);
	///Printer method (on standard output)
	void print();

private:
	/// The Dependency Graph
	Graph depGraph;
	/// The map that containing as keys indices of the predicate,
	/// and as values indices of the vertex in the dependency graph
	unordered_map<index_object, unsigned int> predicateIndexGMap;
};

/*
 *   This class represents the component graph of the program
 */
class ComponentGraph{
public:
	ComponentGraph(){};

	/// Create component graph
	///@param depGraph The dependency graph
	///@param statementAtomMapping The dependency correlation between predicates and rules
	void createComponent(DependencyGraph &depGraph,StatementAtomMapping &statementAtomMapping);

	/// This method adds an edge with the specified weight in the component graph between the two predicate given
	void addEdge(index_object pred_body,index_object pred_head,int weight);
	/// This method compute a single possible ordering among components
	void computeAnOrdering(list<unsigned int>& componentsOrdering);
	/// This method compute all possible orderings among components
	void computeAllPossibleOrdering(vector<vector<unsigned int>>& componentsOrderings);

	///Getter for the components mapping
	const unordered_map<index_object, unsigned int>& getComponent() const {	return component;}

	/// This method prints the dependency graph in a file using DOT standard format
	void printFile(string fileGraph);
	///Printer method (on standard output)
	void print();


private:
	/// The Component Graph
	WeightGraph compGraph;
	///This unordered map maps each predicate to its relative component
	unordered_map<index_object,unsigned int> component;
};

/**
 * 	This class manages the overall creation of the dependency graph
 * 	and the component graph
 */
class StatementDependency {
public:

	StatementDependency(){};

	/// This method determines the predicates appearing in the rule,
	/// stores their mapping, and updates the dependency graph properly according this mapping
	void addRuleMapping(Rule *r);

	/// This method creates the dependency graph
	void createDependencyGraph(PredicateTable* pt);
	/// This method creates the components graph
	void createComponentGraph();

	/// This method creates the components graph and compute an ordering for the components
	/// @param rulesOrdering A vector that will be filled in with rules according to the components ordering
	void createComponentGraphAndComputeAnOrdering(vector<Rule*>& rulesOrdering);

	/// This method returns the number of rules in the program
	unsigned int getRulesSize(){return rules.size();}
	/// Getter for the ith rule
	Rule* getRule(int i) {return rules[i];};

	///Printer method
	void print();

	virtual ~StatementDependency(){for(Rule *r:rules)delete r;};

private:
	/// The Dependency Graph
	DependencyGraph depGraph;
	/// The Component Graph
	ComponentGraph compGraph;
	/// The dependency correlation between predicates and rules
	StatementAtomMapping statementAtomMapping;
	/// The vector of rules composing the program
	vector<Rule*> rules;
};


#endif /* STATEMENTDEPENDENCY_H_ */
