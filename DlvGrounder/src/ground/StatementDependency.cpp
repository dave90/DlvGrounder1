/*
 * StatementDependency.cpp
 *
 *  Created on: 03/apr/2014
 *      Author: Davide
 */

#include "StatementDependency.h"

#include <algorithm>

#include <boost/graph/graph_utility.hpp>

StatementDependency::StatementDependency() {

}

void StatementDependency::addRule(Rule* r) {
	unordered_set<unsigned long> head = r->getPredicateInHead();
	unordered_set<unsigned long> body = r->getPositivePredicateInBody();
	for (unsigned long i : body)
		for (unsigned long j : head)
			if(statementAtomMapping.isInHead(i) && statementAtomMapping.isInHead(j)){
				boost::add_edge(i, j, depGraph);
			}
}

void StatementDependency::addRuleMapping(Rule* r) {
	statementAtomMapping.addRule(r);
}

const vector<Component>& StatementDependency::getOrderedComponents() {
	  using namespace boost;
	  typedef graph_traits<Graph>::vertex_descriptor Vertex;

	  std::vector<int> component(num_vertices(depGraph)), discover_time(num_vertices(depGraph));
	  std::vector<default_color_type> color(num_vertices(depGraph));
	  std::vector<Vertex> root(num_vertices(depGraph));
	  int num = strong_components(depGraph, &component[0],
	                              root_map(&root[0]).
	                              color_map(&color[0]).
	                              discover_time_map(&discover_time[0]));

	  for (unsigned long i = 0; i != component.size(); ++i){
		  if(statementAtomMapping.isInHead(i)){
			 Component *c=new Component(components[i]);
			 auto it=components.find(c);
			 if(it!=components.end()){
				 delete c;

				 auto it1_b=(*it)->rules.begin();
				 auto it1_e=(*it)->rules.end();
				 int current_size=(*it)->rules.size();
				 auto vec_rules=statementAtomMapping.getRuleInBody(i); // FIXME body & end
				 auto it2_b=vec_rules.begin();
				 auto it2_e=vec_rules.end();
				 (*it)->rules.resize(current_size+vec_rules.size());
				 set_union(it1_b,it1_e,it2_b,it2_e,it1_b+current_size);
			 }else{
				 // push rule in component
			 }
		  }
	  }

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

void StatementDependency::printDepGraph() {
    boost::print_graph(depGraph);
}

bool StatementAtomMapping::isInHead(unsigned long p) {
	if(headMap.find(p)!=headMap.end())return true;
	return false;
}

StatementAtomMapping::~StatementAtomMapping() {
}
