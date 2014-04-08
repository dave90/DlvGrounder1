/*
 * StatementDependency.cpp
 *
 *  Created on: 03/apr/2014
 *      Author: Davide
 */

#include "StatementDependency.h"

#include <algorithm>

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/strong_components.hpp>

StatementDependency::StatementDependency() {

}

void StatementDependency::createDependency(vector<Rule*>& rules) {

	// Temp hash_set of predicate
	unordered_set<unsigned long> head_predicateVisited;
	unordered_set<unsigned long> body_predicateVisited;

	for (Rule *r : rules) {

		for (auto head_it = r->getBeginHead(); head_it != r->getEndHead();
				head_it++) {

			unsigned long pred_head = (*head_it)->getPredicate();

			// Verify if the predicate in the head was been visited
			if (!head_predicateVisited.count(pred_head)) {

				// Set this predicate visited
				head_predicateVisited.insert(pred_head);

				for (auto body_it = r->getBeginBody();
						body_it != r->getEndBody(); body_it++) {

					// Verify if the predicate is positive
					if (!(*body_it)->isNegative()) {
						unsigned long pred_body = (*body_it)->getPredicate();

						// Verify if the predicate in the head was been visited and compare in head of same rule
						if (!body_predicateVisited.count(pred_body)	&& statementAtomMapping.isInHead(pred_body)) {

							// Set this predicate visited
							body_predicateVisited.insert(pred_body);
							addEdgeInDependencyGraph(pred_body,pred_head);

						}
					}

				}
			}
			//set unvisited all predicate in the body of the rule
			body_predicateVisited.clear();
		}
		//set unvisited all predicate in the atom of the rule
		head_predicateVisited.clear();
	}

	createComponent(rules);

}

void StatementDependency::calculateStrongComponent() {
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	std::vector<int> discover_time(boost::num_vertices(depGraph));
	component.resize(boost::num_vertices(depGraph));
	std::vector<boost::default_color_type> color(boost::num_vertices(depGraph));
	std::vector<Vertex> root(boost::num_vertices(depGraph));
	boost::strong_components(depGraph, &component[0],
			boost::root_map(&root[0]).color_map(&color[0]).discover_time_map(
					&discover_time[0]));
}

void StatementDependency::createComponent(vector<Rule*>& rules) {

	calculateStrongComponent();

	// Temp hash_set of predicate
	unordered_set<unsigned long> head_predicateVisited;

	for (Rule *r : rules) {

		for (auto head_it = r->getBeginHead(); head_it != r->getEndHead();
				head_it++) {

			unsigned long pred_head = (*head_it)->getPredicate();

			// Verify if the predicate in the head was been visited
			if (!head_predicateVisited.count(pred_head)) {

				// Set this predicate visited
				head_predicateVisited.insert(pred_head);

				for (auto body_it = r->getBeginBody();
						body_it != r->getEndBody(); body_it++) {

					bool isPositive=!(*body_it)->isNegative();


						unsigned long pred_body = (*body_it)->getPredicate();

						// Verify if the predicate compare in head of same rule
						if (statementAtomMapping.isInHead(pred_body)) {
							int weight=isPositive;
							if(!isPositive)weight=-1;
							addEdgeInComponentGraph(pred_body,pred_head,weight);

						}


				}
			}
		}
		//set unvisited all predicate in the atom of the rule
		head_predicateVisited.clear();
	}

}

void StatementDependency::addRuleMapping(Rule* r) {
	statementAtomMapping.addRule(r);
}

void StatementDependency::printDepGraph() {
//	boost::print_graph(depGraph);
	using namespace boost;
	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, depGraph);
	graph_traits<Graph>::edge_iterator ei, ei_end;
	cout << "Dependency Graph:";
	for (tie(ei, ei_end) = edges(depGraph); ei != ei_end; ++ei)
		std::cout << "(" << depGraph[index[source(*ei, depGraph)]].pred_id
				<< "," << depGraph[index[target(*ei, depGraph)]].pred_id
				<< ") ";
	std::cout << std::endl;

}

void StatementDependency::printCompGraph() {
	cout << "Component (predicate,component) : ";
	for (unsigned int i = 0; i < component.size(); i++) {
		cout << "( " << depGraph[i].pred_id << " " << component[i] << ") ";
	}
	cout << endl;
	using namespace boost;
	property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight,
			compGraph);
	typedef graph_traits<Graph>::edge_iterator edge_iter;
	std::pair<edge_iter, edge_iter> ep;
	edge_iter ei, ei_end;
	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, compGraph);
	cout << "Component Graph:";
	for (tie(ei, ei_end) = edges(compGraph); ei != ei_end; ++ei)
		std::cout << "(" << index[source(*ei, compGraph)] << ","
				<< index[target(*ei, compGraph)] << ", " << weightmap[*ei]
				<< ") ";
	cout << endl;

}

void StatementDependency::addEdgeInDependencyGraph(unsigned long pred_body,
		unsigned long pred_head) {
	unsigned int index_i, index_j;
	auto it1 = predicateIndexGMap.find(pred_body);
	auto it2 = predicateIndexGMap.find(pred_head);
	// Calculate if the predicate is present in the graph
	// otherwise assign at the predicate new id (map size)
	if (it1 != predicateIndexGMap.end())
		index_i = it1->second;
	else {
		index_i = predicateIndexGMap.size();
		predicateIndexGMap.insert(
				{ pred_body, index_i });
	}
	if (it2 != predicateIndexGMap.end())
		index_j = it2->second;
	else {
		index_j = predicateIndexGMap.size();
		predicateIndexGMap.insert(
				{ pred_head, index_j });
	}
	boost::add_edge(index_i, index_j, depGraph);
	// Set the predicate in the vertex
	depGraph[index_i].pred_id = pred_body;
	depGraph[index_j].pred_id = pred_head;
}

void StatementDependency::addEdgeInComponentGraph(unsigned long pred_body,
		unsigned long pred_head,int weight) {
	unsigned int pred_index_graph_head = predicateIndexGMap.find(
							pred_head)->second;
	unsigned int pred_index_graph_body =
			predicateIndexGMap.find(pred_body)->second;
		if(component[pred_index_graph_body]!=component[pred_index_graph_head])
			boost::add_edge(component[pred_index_graph_body],
					component[pred_index_graph_head], weight, compGraph);
}

StatementDependency::~StatementDependency() {
}

void StatementAtomMapping::addRule(Rule* r) {
	unordered_set<unsigned long> head = r->getPredicateInHead();
	unordered_set<unsigned long> body = r->getPredicateInBody();
	for (unsigned long i : head) {
		headMap.insert( { i, r });
	}
	for (unsigned long i : body) {
		bodyMap.insert( { i, r });
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

bool StatementAtomMapping::isInHead(unsigned long p) {
	if (headMap.find(p) != headMap.end())
		return true;
	return false;
}

StatementAtomMapping::~StatementAtomMapping() {
}


