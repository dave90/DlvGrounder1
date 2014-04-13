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
#include <boost/graph/graphviz.hpp>
#include <boost/lexical_cast.hpp>

#include "../utility/Config.h"
#include "../table/IdsManager.h"

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

void DependencyGraph::createDependency(vector<Rule*>& rules,
		StatementAtomMapping &statementAtomMapping) {

	// Temp hash_set of predicate
	unordered_set<unsigned long> head_predicateVisited;
	unordered_set<unsigned long> body_predicateVisited;

	for (Rule *r : rules) {

		for (auto head_it = r->getBeginHead(); head_it != r->getEndHead(); head_it++) {

			unsigned long pred_head = (*head_it)->getPredicate();

			// Verify if the predicate in the head was been visited
			if (!head_predicateVisited.count(pred_head)) {

				// Set this predicate visited
				head_predicateVisited.insert(pred_head);

				for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {

					// Verify if the predicate is positive
					if (!(*body_it)->isNegative()) {
						unsigned long pred_body = (*body_it)->getPredicate();

						// Verify if the predicate in the head was been visited and compare in head of same rule
						if (!body_predicateVisited.count(pred_body)
								&& statementAtomMapping.isInHead(pred_body)) {

							// Set this predicate visited
							body_predicateVisited.insert(pred_body);
							addEdge(pred_body, pred_head);

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

}

void DependencyGraph::addInDependency(Rule* r) {

	// Temp hash_set of predicate
	unordered_set<unsigned long> head_predicateVisited;
	unordered_set<unsigned long> body_predicateVisited;

	for (auto head_it = r->getBeginHead(); head_it != r->getEndHead(); head_it++) {

		unsigned long pred_head = (*head_it)->getPredicate();

		// Verify if the predicate in the head was been visited
		if (!head_predicateVisited.count(pred_head)) {

			// Set this predicate visited
			head_predicateVisited.insert(pred_head);

			for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {

				// Verify if the predicate is positive
				if (!(*body_it)->isNegative()) {
					unsigned long pred_body = (*body_it)->getPredicate();

					// Verify if the predicate in the head was been visited
					if (!body_predicateVisited.count(pred_body)) {

						// Set this predicate visited
						body_predicateVisited.insert(pred_body);
						addEdge(pred_body, pred_head);

					}
				}

			}
		}
		//set unvisited all predicate in the body of the rule
		body_predicateVisited.clear();
	}

}

void DependencyGraph::deleteVertex(unordered_set<unsigned long>& delete_pred) {
	boost::graph_traits<Graph>::vertex_iterator vi, vi_end, next;
	tie(vi, vi_end) = boost::vertices(depGraph);
	for (next = vi; vi != vi_end; vi = next) {
	  ++next;
	   if (delete_pred.count(depGraph[*vi].pred_id)){
		   remove_vertex(*vi, depGraph);
	   }
	}
}

void DependencyGraph::print() {
	using namespace boost;

	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, depGraph);
	graph_traits<Graph>::edge_iterator ei, ei_end;
	string graphDOT = "digraph Dependency_Graph{\n";

	// Print the edge
	for (tie(ei, ei_end) = edges(depGraph); ei != ei_end; ++ei) {
		unsigned long p1 = index[source(*ei, depGraph)];
		unsigned long p2 = index[target(*ei, depGraph)];
		graphDOT += lexical_cast<string>(p1) + "->" + lexical_cast<string>(p2) + ";\n";
	}

	//Print label  (the name of the predicate)
	for (unsigned int i = 0; i < num_vertices(depGraph); i++) {
		graphDOT += lexical_cast<string>(i) + " [label= \"";
		string predicate = IdsManager::getString(IdsManager::PREDICATE_ID_MANAGER,
				depGraph[i].pred_id);
		graphDOT += predicate.substr(0, predicate.find("*")) + "  " + "\"];\n";
	}
	graphDOT += "}\n";

	string fileGraph = Config::getInstance()->getFileGraph() + "DG";
	if (strcmp(fileGraph.c_str(), "DG") == 0) {
		cout << graphDOT << endl;
	} else {
		ofstream myfile;
		myfile.open(fileGraph);
		myfile << graphDOT;
		myfile.close();
		string COMMAND = "dot -Tpng " + fileGraph + " -O";
		system(COMMAND.c_str());
		remove(fileGraph.c_str());
	}

}

void DependencyGraph::addEdge(unsigned long pred_body, unsigned long pred_head) {
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

void DependencyGraph::calculateStrongComponent(
		unordered_map<unsigned long, unsigned int> &component) {
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	std::vector<int> discover_time(boost::num_vertices(depGraph));
	vector<unsigned int> component_indexed;
	component_indexed.resize(boost::num_vertices(depGraph));
	std::vector<boost::default_color_type> color(boost::num_vertices(depGraph));
	std::vector<Vertex> root(boost::num_vertices(depGraph));
	boost::strong_components(depGraph, &component_indexed[0],
			boost::root_map(&root[0]).color_map(&color[0]).discover_time_map(&discover_time[0]));
	for (unsigned int i = 0; i < component_indexed.size(); i++) {
		component.insert( { depGraph[i].pred_id, component_indexed[i] });
	}
}

void ComponentGraph::addEdge(unsigned long pred_body, unsigned long pred_head, int weight) {

	if (component[pred_body] != component[pred_head])
		boost::add_edge(component[pred_body], component[pred_head], weight, compGraph);
}

void ComponentGraph::createComponent(vector<Rule*>& rules, DependencyGraph &depGraph,
		StatementAtomMapping &statementAtomMapping) {
	depGraph.calculateStrongComponent(component);

	// Temp hash_set of predicate
	unordered_set<unsigned long> head_predicateVisited;

	for (Rule *r : rules) {

		for (auto head_it = r->getBeginHead(); head_it != r->getEndHead(); head_it++) {

			unsigned long pred_head = (*head_it)->getPredicate();

			// Verify if the predicate in the head was been visited
			if (!head_predicateVisited.count(pred_head)) {

				// Set this predicate visited
				head_predicateVisited.insert(pred_head);

				for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {

					bool isPositive = !(*body_it)->isNegative();

					unsigned long pred_body = (*body_it)->getPredicate();

					// Verify if the predicate compare in head of same rule
					if (statementAtomMapping.isInHead(pred_body)) {
						int weight = isPositive;
						if (!isPositive)
							weight = -1;
						addEdge(pred_body, pred_head, weight);

					}

				}
			}
		}
		//set unvisited all predicate in the atom of the rule
		head_predicateVisited.clear();
	}

}

void ComponentGraph::print() {

	string graphDOT = "digraph Component_Graph {\n";

	using namespace boost;
	property_map<WeightGraph, edge_weight_t>::type weightmap = get(edge_weight, compGraph);
	typedef graph_traits<WeightGraph>::edge_iterator edge_iter;
	std::pair<edge_iter, edge_iter> ep;
	edge_iter ei, ei_end;
	typedef property_map<WeightGraph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, compGraph);

	// Print the edge
	for (tie(ei, ei_end) = edges(compGraph); ei != ei_end; ++ei) {
		graphDOT += lexical_cast<string>(index[source(*ei, compGraph)]) + "->"
				+ lexical_cast<string>(index[target(*ei, compGraph)]) + "[color= ";
		// negative RED , BLUE positive
		if (weightmap[*ei] > 0)
			graphDOT += "blue";
		else
			graphDOT += "red";
		graphDOT += "];\n";
	}

	//Print the label (the predicates in the component)
	for (unsigned int i = 0; i < num_vertices(compGraph); i++) {
		graphDOT += lexical_cast<string>(i);
		graphDOT += " [label= \"";
		for (auto it : component)
			if (it.second == i) {
				string predicate = IdsManager::getString(IdsManager::PREDICATE_ID_MANAGER,
						it.first);
				graphDOT += predicate.substr(0, predicate.find("*")) + "  ";
			}
		graphDOT += "\"];\n";
	}

	graphDOT += "}\n";

	string fileGraph = Config::getInstance()->getFileGraph() + "CG";
	if (strcmp(fileGraph.c_str(), "CG") == 0) {
		cout << graphDOT << endl;
	} else {
		ofstream myfile;
		myfile.open(fileGraph);
		myfile << graphDOT;
		myfile.close();
		string COMMAND = "dot -Tpng " + fileGraph + " -O";
		system(COMMAND.c_str());
		remove(fileGraph.c_str());
	}
}

void StatementDependency::addRuleMapping(Rule* r) {
	statementAtomMapping.addRule(r);
	rules.push_back(r);

	depGraph.addInDependency(r);
}

void StatementDependency::createDependencyGraph(PredicateTable* pt) {
//	depGraph.createDependency(rules, statementAtomMapping);
	unordered_set<unsigned long> delete_pred;
	pt->getEdbPredicate(delete_pred);
	depGraph.deleteVertex(delete_pred);
}

void StatementDependency::createComponentGraph() {
	compGraph.createComponent(rules, depGraph, statementAtomMapping);
}


void StatementDependency::print() {
	if (Config::getInstance()->isDependency())
		depGraph.print();
	if (Config::getInstance()->isComponent())
		compGraph.print();
}

