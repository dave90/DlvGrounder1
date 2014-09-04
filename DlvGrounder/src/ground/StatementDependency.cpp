/*
 * StatementDependency.cpp
 *
 *  Created on: 03/apr/2014
 *      Author: Davide
 */

#include "StatementDependency.h"
#include "../utility/Config.h"
#include "../table/IdsManager.h"

#include <algorithm>

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/topological_sort.hpp>


void StatementAtomMapping::addRule(Rule* r) {
	unordered_set<index_object> head = r->getPredicateInHead();
	unordered_set<index_object> body = r->getPredicateInBody();
	for (index_object i : head) {
		headMap.insert( { i, r });
	}
	for (index_object i : body) {
		bodyMap.insert( { i, r });
	}
}

void StatementAtomMapping::getRuleInHead(index_object p, vector<Rule*>& rules) {
	auto pair1 = headMap.equal_range(p);
	for (; pair1.first != pair1.second; ++pair1.first)
		rules.push_back(pair1.first->second);
}

const vector<Rule*> StatementAtomMapping::getRuleInBody(index_object p) {
	vector<Rule*> rules;
	auto pair1 = bodyMap.equal_range(p);
	for (; pair1.first != pair1.second; ++pair1.first)
		rules.push_back(pair1.first->second);
	return rules;
}

bool StatementAtomMapping::isInHead(index_object p) {
	if (headMap.find(p) != headMap.end())
		return true;
	return false;
}

StatementAtomMapping::~StatementAtomMapping() {}

/*
 *
 *
 *
 *
 * 	END StatementAtomMapping
 *
 *
 *
 *
 *
 */

void DependencyGraph::addInDependency(Rule* r) {

	// Temporary set of predicates
	unordered_set<index_object> head_predicateVisited;
	unordered_set<index_object> body_predicateVisited;

	for (auto head_it = r->getBeginHead(); head_it != r->getEndHead(); head_it++) {
		pair<bool, index_object> pred_head = (*head_it)->getPredicate();

		// Check if the predicate in the head has been visited
		if (pred_head.first && !head_predicateVisited.count(pred_head.second)) {

			// Set this predicate as visited
			head_predicateVisited.insert(pred_head.second);

			// If the rule has no positive predicate in its body, is added a dummy edge in order to not lose this rule
			// when the components graph is created
			if(r->getPositivePredicateInBody().size()==0)
				addEdge(pred_head.second, pred_head.second,1);

			for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {

				// Check if the predicate is positive, otherwise skip it
				if (!(*body_it)->isNegative()) {
					pair<bool,index_object> pred_body = (*body_it)->getPredicate();

					// Check if the predicate in the head has been visited
					if (pred_body.first  && !body_predicateVisited.count(pred_body.second)) {

						// Set this predicate as visited
						body_predicateVisited.insert(pred_body.second);
						addEdge(pred_body.second, pred_head.second,1);

					}
				}
				else{
					pair<bool,index_object> pred_body = (*body_it)->getPredicate();

					// Check if the predicate in the head has been visited
					if (pred_body.first  && !body_predicateVisited.count(pred_body.second)) {

						// Set this predicate as visited
						body_predicateVisited.insert(pred_body.second);
						addEdge(pred_body.second, pred_head.second,-1);

					}
				}

			}
		}
		//Set all predicate in the body as unvisited, and then continue with the next atom
		body_predicateVisited.clear();
	}

}

void DependencyGraph::deleteVertex(unordered_set<index_object>& delete_pred) {

	while (delete_pred.size() > 0) {
		// Boost actually re-indexes the vertices after one vertex has been deleted.
		// So it is needed to find again the new vertex index for each predicate.
		index_object current_pred = *delete_pred.begin();
		boost::graph_traits<Graph>::vertex_iterator vi, vi_end, next;
		tie(vi, vi_end) = boost::vertices(depGraph);
		for (next = vi; vi != vi_end; vi = next) {
			++next;
			if (current_pred == depGraph[*vi].pred_id) {
				remove_vertex(*vi, depGraph);
				break;
			}
		}
		delete_pred.erase(current_pred);
	}
}

void DependencyGraph::printFile(string fileGraph) {
	using namespace boost;

	typedef property_map<Graph, vertex_index_t>::type IndexMap;
//	IndexMap index = get(vertex_index, depGraph);
	IndexMap index = get(vertex_index, depGraph);
	graph_traits<Graph>::edge_iterator ei, ei_end;
	string graphDOT = "digraph Dependency_Graph{\n";

	// Print the edges
	for (tie(ei, ei_end) = edges(depGraph); ei != ei_end; ++ei) {
		index_object p1 = index[source(*ei, depGraph)];
		index_object p2 = index[target(*ei, depGraph)];
		graphDOT += lexical_cast<string>(p1) + "->" + lexical_cast<string>(p2) + ";\n";
	}

	//Print labels  (the name of the predicate)
	for (unsigned int i = 0; i < num_vertices(depGraph); i++) {
		graphDOT += lexical_cast<string>(i) + " [label= \"";
		string predicate = IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,
				depGraph[i].pred_id);
		graphDOT += predicate + "  " + "\"];\n";
	}
	graphDOT += "}\n";

	ofstream myfile;
	myfile.open(fileGraph);
	myfile << graphDOT;
	myfile.close();
	string COMMAND = "dot -Tpng " + fileGraph + " -O";
	system(COMMAND.c_str());
	remove(fileGraph.c_str());

}

void DependencyGraph::print() {
	using namespace boost;

	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, depGraph);
	graph_traits<Graph>::edge_iterator ei, ei_end;

	// Print the edges
	for (tie(ei, ei_end) = edges(depGraph); ei != ei_end; ++ei) {
		index_object p1 = index[source(*ei, depGraph)];
		index_object p2 = index[target(*ei, depGraph)];
		cout << "("
				<< IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,
						depGraph[p1].pred_id) << ","
				<< IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,
						depGraph[p2].pred_id) << ")" << " ";
	}

}

void DependencyGraph::addEdge(index_object pred_body, index_object pred_head, int weight) {
	unsigned int index_i, index_j;
	auto it1 = predicateIndexGMap.find(pred_body);
	// Check if each predicate is already present in the graph
	// otherwise assign at to it a new id, for this purpose the map size is used
	if (it1 != predicateIndexGMap.end())
		index_i = it1->second;
	else {
		index_i = predicateIndexGMap.size();
		predicateIndexGMap.insert( { pred_body, index_i });
	}
	auto it2 = predicateIndexGMap.find(pred_head);
	if (it2 != predicateIndexGMap.end())
		index_j = it2->second;
	else {
		index_j = predicateIndexGMap.size();
		predicateIndexGMap.insert( { pred_head, index_j });
	}
	boost::add_edge(index_i, index_j, weight, stratifiedGraph);
	stratifiedGraph[index_i].pred_id = pred_body;
	stratifiedGraph[index_j].pred_id = pred_head;
	if(weight>0){
		boost::add_edge(index_i, index_j, depGraph);
		// Set the predicate in the vertex
		depGraph[index_i].pred_id = pred_body;
		depGraph[index_j].pred_id = pred_head;
	}
}

void DependencyGraph::calculateStrongComponent(unordered_map<index_object, unsigned int> &componentDepependency) {

	using namespace boost;
	typedef graph_traits<Graph>::vertex_descriptor Vertex;

	std::vector<int> component_indices(num_vertices(depGraph)), discover_time(num_vertices(depGraph));
	std::vector<default_color_type> color(num_vertices(depGraph));
	std::vector<Vertex> root(num_vertices(depGraph));
	strong_components(depGraph, make_iterator_property_map(component_indices.begin(), get(vertex_index, depGraph)),
							  root_map(make_iterator_property_map(root.begin(), get(vertex_index, depGraph))).
							  color_map(make_iterator_property_map(color.begin(), get(vertex_index, depGraph))).
							  discover_time_map(make_iterator_property_map(discover_time.begin(), get(vertex_index, depGraph))));

	for (unsigned int i = 0; i < component_indices.size(); i++)
		componentDepependency.insert( { depGraph[i].pred_id, component_indices[i] });



}


void DependencyGraph::calculateUnstritifiedPredicate(unordered_set<index_object>& predicateUnstratified){
	using namespace boost;

	//Calculate the cycle in stratifiedGraph and verify thath not exist recursion and negation, if exist
	// then all the predicate in that cycle are not stratified

	typedef graph_traits<WeightGraph>::vertex_descriptor VertexWeight;

	std::vector<int> component_indices_weight(num_vertices(stratifiedGraph)), discover_time_weight(num_vertices(stratifiedGraph));
	std::vector<default_color_type> color_weight(num_vertices(stratifiedGraph));
	std::vector<VertexWeight> root_weight(num_vertices(stratifiedGraph));
	strong_components(stratifiedGraph, make_iterator_property_map(component_indices_weight.begin(), get(vertex_index, stratifiedGraph)),
							  root_map(make_iterator_property_map(root_weight.begin(), get(vertex_index, stratifiedGraph))).
							  color_map(make_iterator_property_map(color_weight.begin(), get(vertex_index, stratifiedGraph))).
							  discover_time_map(make_iterator_property_map(discover_time_weight.begin(), get(vertex_index, stratifiedGraph))));

	typedef property_map<WeightGraph, vertex_index_t>::type IndexMap;
	property_map<WeightGraph, edge_weight_t>::type weightmap = get(edge_weight, stratifiedGraph);
	IndexMap index = get(vertex_index, depGraph);
	graph_traits<WeightGraph>::edge_iterator ei, ei_end;

	std::unordered_set<unsigned int> component_processed;
	for (unsigned int i = 0; i < component_indices_weight.size(); i++){
//		cout<<IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,stratifiedGraph[i].pred_id)<<" "<<component_indices_weight[i]<<endl;
		bool next=false;
		if(!component_processed.count(component_indices_weight[i])){
					for (tie(ei, ei_end) = edges(stratifiedGraph); ei != ei_end && !next; ++ei) {
						index_object p1 = stratifiedGraph[index[source(*ei, stratifiedGraph)]].pred_id;
						index_object i2 = index[target(*ei, stratifiedGraph)];
						// If in this component not find a negative arc
						// search if exist a negative arc with an edge in this component
						if(stratifiedGraph[i].pred_id==p1 && component_indices_weight[i]==component_indices_weight[i2] && weightmap[*ei]<0){
							component_processed.insert(component_indices_weight[i]);
							for (unsigned int k = 0; k < component_indices_weight.size(); k++)
								if(component_indices_weight[k]==component_indices_weight[i])predicateUnstratified.insert(stratifiedGraph[k].pred_id);
							next=true;
						}
					}
		}
	}
//		for(auto stratPred:predicateUnstratified)
//			cout<<"UNDEF "<<IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,stratPred)<<endl;
}

/*
 *
 *
 *
 *
 * 	END DependencyGraph
 *
 *
 *
 *
 *
 */

void ComponentGraph::addEdge(index_object pred_body, index_object pred_head, int weight) {

	if (componentDependency[pred_body] != componentDependency[pred_head])
		boost::add_edge(componentDependency[pred_body], componentDependency[pred_head], weight, compGraph);
}

void ComponentGraph::createComponent(DependencyGraph &depGraph,
		StatementAtomMapping &statementAtomMapping) {
	depGraph.calculateStrongComponent(componentDependency);
	depGraph.calculateUnstritifiedPredicate(predicateUnstratified);

	// For each component create a vertex in a graph
	// because when adding a edge the vertex have to be created and if exist one component
	// with a body in a rule with all fact are not skipped

	for (auto it : componentDependency)
		boost::add_vertex(compGraph);

	vector<Rule*> rules;
	for (auto it : componentDependency) {

		index_object pred_head = it.first;


		statementAtomMapping.getRuleInHead(pred_head, rules);

		for (Rule *r : rules) {

			for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {

				bool isPositive = !(*body_it)->isNegative();

				index_object pred_body = (*body_it)->getPredicate().second;

				// Check if the predicate appear in the head also
				if (statementAtomMapping.isInHead(pred_body)) {
					int weight = isPositive;
					if (!isPositive)
						weight = -1;
					// FIXME if put negative edge check if exist positive edge
					addEdge(pred_body, pred_head, weight);

				}

			}

		}
		rules.clear();
	}


}

bool ComponentGraph::isPredicateNegativeStratified(index_object predicate){
	return predicateUnstratified.count(predicate);
}

void ComponentGraph::printFile(string fileGraph) {

	string graphDOT = "digraph Component_Graph {\n";

	using namespace boost;
	property_map<WeightGraph, edge_weight_t>::type weightmap = get(edge_weight, compGraph);
	typedef graph_traits<WeightGraph>::edge_iterator edge_iter;
	std::pair<edge_iter, edge_iter> ep;
	edge_iter ei, ei_end;
	typedef property_map<WeightGraph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, compGraph);

	// Print the edges
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

	//Print the labels (the predicates in the component)
	for (unsigned int i = 0; i < num_vertices(compGraph); i++) {
		graphDOT += lexical_cast<string>(i);
		graphDOT += " [label= \"";
		for (auto it : componentDependency)
			if (it.second == i) {
				string predicate = IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,
						it.first);
				graphDOT += predicate + "  ";
			}
		graphDOT += "\"];\n";
	}

	graphDOT += "}\n";

	ofstream myfile;
	myfile.open(fileGraph);
	myfile << graphDOT;
	myfile.close();
	string COMMAND = "dot -Tpng " + fileGraph + " -O";
	system(COMMAND.c_str());
	remove(fileGraph.c_str());

}

void ComponentGraph::print() {
	for (unsigned int i = 0; i < num_vertices(compGraph); i++) {
		cout << "{ ";
		bool first = false;
		for (auto it : componentDependency)
			if (it.second == i) {
				string predicate = IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,
						it.first);
				if (!first) {
					cout << predicate + " ";
					first = true;
				} else
					cout << ", " << predicate << " ";

			}
		cout << "} ";
	}
}

void ComponentGraph::computeAnOrdering(list<unsigned int>& componentsOrdering) {

	// If the component graph is not connected, then if there are some components any ordering is valid
	if(num_vertices(compGraph)>=0 && num_edges(compGraph)==0 && componentDependency.size()>0){
		unordered_set<unsigned int> components;
		for(auto pair:componentDependency)
			components.insert(pair.second);
		for(auto it:components)
			componentsOrdering.push_back(it);
		return;
	}

	// If the component graph is a-cyclic is suffices to compute a topological sort to get a valid components ordering
	try {
		topological_sort(compGraph, front_inserter(componentsOrdering));
	// Otherwise an not_a_dag exception is thrown, and it is computed an ordering taking into account that the graph is cyclic
	} catch (boost::not_a_dag const& e) {
		this->recursive_sort(componentsOrdering);
	}

//	printTheOrderingOfComponent(componentsOrdering);
}

void ComponentGraph::printTheOrderingOfComponent(list<unsigned int>& componentsOrdering){
	cout<<"ORDER:"<<endl;
	for(auto comp:componentsOrdering){
		cout<<"Component "<<comp<<" ";
		for(auto it:componentDependency)
			if(it.second==comp)
				cout<<IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,it.first);
		cout<<endl;
	}
}


void ComponentGraph::recursive_sort(list<unsigned int>& componentsOrdering) {
	// Recursive method:
	// If exist a cycle detect the component of each cycle and for each cycle delete one
	// negative edge that connect two vertices of the same cycle, if topological sort don't report
	// error then exit else redo all the operation until all the cycle are deleted


	// Calculate the strong component (the cycle in the graph)
	using namespace boost;
	property_map<WeightGraph, edge_weight_t>::type weightmap = get(edge_weight, compGraph);
	typedef graph_traits<WeightGraph>::edge_iterator edge_iter;
	std::pair<edge_iter, edge_iter> ep;
	edge_iter ei, ei_end;
	vector<edge_iter> edgeToRemove;
	typedef property_map<WeightGraph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, compGraph);


	typedef graph_traits<WeightGraph>::vertex_descriptor VertexWeight;

	std::vector<int> component_indices_weight(num_vertices(compGraph)), discover_time_weight(num_vertices(compGraph));
	std::vector<default_color_type> color_weight(num_vertices(compGraph));
	std::vector<VertexWeight> root_weight(num_vertices(compGraph));
	strong_components(compGraph, make_iterator_property_map(component_indices_weight.begin(), get(vertex_index, compGraph)),
							  root_map(make_iterator_property_map(root_weight.begin(), get(vertex_index, compGraph))).
							  color_map(make_iterator_property_map(color_weight.begin(), get(vertex_index, compGraph))).
							  discover_time_map(make_iterator_property_map(discover_time_weight.begin(), get(vertex_index, compGraph))));

	// For each element of each component if find a negative edge that start with the element and finish with a vertex with the same
	// component then delete the edge and process another component
	std::unordered_set<unsigned int> componentProcessed;
	for(unsigned int i=0;i<component_indices_weight.size();i++){
		if(componentProcessed.count(component_indices_weight[i]))continue;
		for (tie(ei, ei_end) = edges(compGraph); ei != ei_end; ++ei) {
			if (weightmap[*ei] < 0 && index[source(*ei, compGraph)]==i && component_indices_weight[index[source(*ei, compGraph)]]==component_indices_weight[i]){
				edgeToRemove.push_back(ei);
				componentProcessed.insert(component_indices_weight[i]);
				break;
			}
		}
	}

	for(auto edge:edgeToRemove)
		remove_edge(*edge,compGraph);
	try{
		topological_sort(compGraph, front_inserter(componentsOrdering));
	} catch (boost::not_a_dag const& e) {
		componentsOrdering.clear();
		this->recursive_sort(componentsOrdering);
	}


}


void ComponentGraph::computeAllPossibleOrdering(vector<vector<unsigned int>>& componentsOrderings) {
	//TODO
}

/*
 *
 *
 *
 * END ComponentGraph
 *
 *
 *
 *
 *
 */

void StatementDependency::addRuleMapping(Rule* r) {
	if(r->isAStrongConstraint()) constraints.push_back(r);
	else{
		statementAtomMapping.addRule(r);
		rules.push_back(r);
		depGraph.addInDependency(r);
	}
}

void StatementDependency::createDependencyGraph(PredicateTable* pt) {
	unordered_set<index_object> delete_pred;
	pt->getEdbPredicate(delete_pred);
	depGraph.deleteVertex(delete_pred);
}

void StatementDependency::createComponentGraph() {
	compGraph.createComponent(depGraph, statementAtomMapping);
}

// An utility function that specifies how to sort the rules of a component
bool sortRules (Rule* r1,Rule* r2) {

	unordered_set<index_object> headSecondRule=r2->getPredicateInHead();
	for(auto it=r1->getBeginBody();it!=r1->getEndBody();it++)
		if(headSecondRule.count((*it)->getPredicate().second))
			return false;
	return true;

}

void StatementDependency::createComponentGraphAndComputeAnOrdering(vector<vector<Rule*>>& exitRules, vector<vector<Rule*>>& recursiveRules,
		vector<unordered_set<index_object>>& componentPredicateInHead) {
	/// Create the component graph
	compGraph.createComponent(depGraph, statementAtomMapping);

	/// Compute a possible ordering among components
	list<unsigned int> ordering;
	compGraph.computeAnOrdering(ordering);

	/// Declaration of some temporary variables
	vector<Rule*> componentsRules;
	int i=0;

	unordered_map<index_object, unsigned int> components=compGraph.getComponent();
	unordered_set<Rule*> addedRules;

	for(auto comp: ordering){
		exitRules.push_back(vector<Rule*>());
		recursiveRules.push_back(vector<Rule*>());
		componentPredicateInHead.push_back(unordered_set<index_object>());

		for (auto pair: components)
			if(pair.second==comp){
				/// Get all the rules for the current component
				index_object predicate=pair.first;
				statementAtomMapping.getRuleInHead(predicate,componentsRules);
				/// For each rule classify it as exit or recursive
				for(Rule* r: componentsRules){
					if(addedRules.insert(r).second){
						if(checkIfExitRule(comp,r))
							exitRules[i].push_back(r);
						else{
							recursiveRules[i].push_back(r);
							componentPredicateInHead[i].insert(r->getPredicateInHead().begin(),r->getPredicateInHead().end());
						}
					}
				}
				componentsRules.clear();
			}
		i++;
	}

	for(unsigned int i=0;i<recursiveRules.size();i++)
		sort(recursiveRules[i].begin(),recursiveRules[i].end(),sortRules);


}


bool StatementDependency::checkIfExitRule(unsigned int component, Rule* rule){
	unordered_set<index_object> positivePredicates=rule->getPositivePredicateInBody();
	unordered_map<index_object, unsigned int> components=compGraph.getComponent();

	for (auto pair: components)
		if(pair.second==component && positivePredicates.count(pair.first))
			return false;
	return true;
}

void StatementDependency::print() {
	string fileDGraph = Config::getInstance()->getFileGraph() + "DG";
	string fileCGraph = Config::getInstance()->getFileGraph() + "CG";

	if (Config::getInstance()->isDependency()) {
		if (strcmp(fileDGraph.c_str(), "DG") == 0)
			depGraph.print();
		else
			depGraph.printFile(fileDGraph);
	}
	if (Config::getInstance()->isComponent()) {
		if (strcmp(fileCGraph.c_str(), "CG") == 0)
			compGraph.print();
		else
			compGraph.printFile(fileCGraph);

	}
	if (Config::getInstance()->isPrintRules())
		for (Rule*r : rules)
			r->print();

}
