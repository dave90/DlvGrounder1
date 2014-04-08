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

	vector<unsigned long> bodyInHead;
	for (Rule *r : rules) {
		unordered_set<unsigned long> head = r->getPredicateInHead();
		unordered_set<unsigned long> body = r->getPositivePredicateInBody();

		// Calculate predicate in body that compare in head
		for (unsigned long pred_body : body)
			if (statementAtomMapping.isInHead(pred_body))
				bodyInHead.push_back(pred_body);

		for (unsigned long pred_head : head)
			for (unsigned long pred_body : bodyInHead) {
				// Index in the graph
				unsigned int index_i, index_j;
				auto it1 = predicateIndexGMap.find(pred_body);
				auto it2 = predicateIndexGMap.find(pred_head);
				// Calculate if the predicate is present in the graph
				// otherwise assign at the predicate new id (map size)
				if (it1 != predicateIndexGMap.end())
					index_i = it1->second;
				else {
					index_i = predicateIndexGMap.size();
					predicateIndexGMap.insert( { pred_body, index_i });
				}
				if (it2 != predicateIndexGMap.end())
					index_j = it2->second;
				else {
					index_j = predicateIndexGMap.size();
					predicateIndexGMap.insert( { pred_head, index_j });
				}
				boost::add_edge(index_i, index_j, depGraph);
				// Set the predicate in the vertex
				depGraph[index_i].pred_id = pred_body;
				depGraph[index_j].pred_id = pred_head;
			}
		bodyInHead.clear();
	}
	createComponent(rules);

}

void StatementDependency::createComponent(vector<Rule*>& rules) {

	// Calculate the strong components with boost function
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	std::vector<int> discover_time(
			boost::num_vertices(depGraph));
	component.resize(boost::num_vertices(depGraph));
	std::vector<boost::default_color_type> color(boost::num_vertices(depGraph));
	std::vector<Vertex> root(boost::num_vertices(depGraph));
	boost::strong_components(depGraph, &component[0],
			boost::root_map(&root[0]).color_map(&color[0]).discover_time_map(
					&discover_time[0]));

	// Create component graph
	vector<unsigned long> bodyInHead;
	for (Rule *r : rules) {
		unordered_set<unsigned long> head = r->getPredicateInHead();
		unordered_set<unsigned long> body = r->getPredicateInBody();
		//TODO optimize this
		unordered_set<unsigned long> positive_body = r->getPositivePredicateInBody();

		// Calculate predicate in body that compare in head
		for (unsigned long pred_body : body)
			if (statementAtomMapping.isInHead(pred_body))
				bodyInHead.push_back(pred_body);

		for (unsigned long pred_head : head)
			for (unsigned long pred_body : bodyInHead) {
				// Take the index of the predicate in the depGraph
				unsigned int pred_index_graph_head =
						predicateIndexGMap.find(pred_head)->second;
				unsigned int pred_index_graph_body =
						predicateIndexGMap.find(pred_body)->second;
				// TODO if there is positive and negative arc????
				if (component[pred_index_graph_head] != component[pred_index_graph_body]){
					if(positive_body.find(pred_body)!=positive_body.end())
					boost::add_edge(component[pred_index_graph_body],
							component[pred_index_graph_head], 1,compGraph);
					else
						boost::add_edge(component[pred_index_graph_body],
												component[pred_index_graph_head], -1,compGraph);
				}
			}
		bodyInHead.clear();
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
	cout<<"Component (predicate,component) : ";
	for(unsigned int i=0;i<component.size();i++){
		cout<<"( "<<depGraph[i].pred_id<<" "<<component[i]<<") ";
	}
	cout<<endl;
	using namespace boost;
	property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, compGraph);
	typedef graph_traits<Graph>::edge_iterator edge_iter;
	std::pair<edge_iter, edge_iter> ep;
	edge_iter ei, ei_end;
	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, compGraph);
	cout << "Component Graph:";
	for (tie(ei, ei_end) = edges(compGraph); ei != ei_end; ++ei)
		std::cout << "(" << index[source(*ei, compGraph)]
				<< "," << index[target(*ei, compGraph)]
				<< ", " <<  weightmap[*ei]
				<< ") " ;
	cout<< endl ;

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

