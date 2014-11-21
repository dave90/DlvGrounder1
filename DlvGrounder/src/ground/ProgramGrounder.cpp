/*
 * ProgramGrounder.cpp
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#define DEBUG 0

#include <list>

#include "ProgramGrounder.h"
#include "../atom/ClassicalLiteral.h"
#include "../utility/Timer.h"
void ProgramGrounder::ground() {

	//Create the dependency graph
	statementDependency->createDependencyGraph(predicateTable);

	// Create the component graph and compute an ordering among components.
	// Components' rules are classified as exit or recursive.
	// An rule occurring in a component is recursive if there is a predicate belonging
	// to the component in its positive body, otherwise it is said to be an exit rule.
	vector<vector<Rule*>> exitRules;
	vector<vector<Rule*>> recursiveRules;
	vector<unordered_set<index_object>> componentPredicateInHead;
	statementDependency->createComponentGraphAndComputeAnOrdering(exitRules, recursiveRules, componentPredicateInHead);

	printFact();

	Timer::getInstance()->start("Ground Rule");
	// Ground each module according to the ordering:
	// For each component, each rule is either recursive or exit,
	// Exit rules are grounded just once, while recursive rules are grounded until no more knowledge is derived
	for (unsigned int component = 0; component < exitRules.size(); component++) {

#if DEBUG == 1
		cout<<"Component: "<<component;
		cout<<"\tExit rules: "<<exitRules[component].size();
		cout<<"\tRecursive rules: "<<recursiveRules[component].size()<<endl;
#endif
		// Ground exit rules
		for (Rule* r : exitRules[component])
			groundRule(r, false, false, nullptr);

		// Ground recursive rules
		if (recursiveRules[component].size() > 0) {
			bool finish = false;
			bool firstIteration = true;
			unsigned int n_rules = recursiveRules[component].size();

			while (!finish && n_rules > 0) {
				bool end = true;
				// Since in the first iteration search is performed in facts and no facts tables,
				// while in the next iteration search is performed in the delta table, it is needed
				// to keep track if the current iteration is the first or not.
				for (unsigned int i = 0; i < n_rules; i++) {
					Rule* r = recursiveRules[component][i];
					//If no more knowledge is derived the grounding of this component can stop
#if DEBUG == 1
					r->print();
#endif
					if (r->getSizeBody() > 0)
						if (groundRule(r, firstIteration, true, &componentPredicateInHead[component]))
							end = false;
				}
#if DEBUG == 1
				cout<<"First Iteration: "<<firstIteration<<" Finish: "<<end<<endl;
#endif
				finish = end;
				if (!firstIteration) {
					for (unsigned int i = 0; i < n_rules; i++)
						// Move the content of the delta table in the no fact table,
						// and fill delta with the content of the next delta table.
						updateDelta(recursiveRules[component][i]);
				}
				if (firstIteration)
					firstIteration = false;
			}
		}
	}
	// Constraints are grounded at the end
	for (unsigned int i = 0; i < statementDependency->getConstraintSize(); i++)
		if (statementDependency->getConstraint(i)->getSizeBody() > 0)
			groundRule(statementDependency->getConstraint(i), false, false, nullptr);

	//Print and simplify the rule
	evaluator.printAndSimplify(instancesTable);

	Timer::getInstance()->end();
}

void ProgramGrounder::updateDelta(Rule* r) {
	for (auto it = r->getBeginHead(); it != r->getEndHead(); it++) {
		Instances* is = instancesTable->getInstance((*it)->getPredicate());
		if (is != nullptr)
			is->moveNextDeltaInDelta();
	}
}

bool ProgramGrounder::groundBoundAtom(bool &find, bool negation, bool searchDelta, Instances* instance, Atom*& templateAtom) {

	current_id_match.push_back(0);
	//  If it is a built in atom, ground it and evaluate it (Built in have not instance table, since they have not a predicate)
	if (current_atom->isBuiltIn()) {
		removeBindValueInAssignment(current_variables_atoms[index_current_atom]);
		Atom* groundBuiltIn = current_atom->ground(current_var_assign);
		find = groundBuiltIn->evaluate(current_var_assign);
		delete groundBuiltIn;
		// If there isn't instances the search fails (find equal false) and if isn't negated then the algorithm have to stop
	} else if (instance == nullptr) {
		if (!negation)
			return false;
		else
			find = true;
	} else {
		if (templateAtom != nullptr)
			delete templateAtom;

		templateAtom = setBoundValue();
		bool isUndef;
		instance->getIndex()->findIfExist(searchDelta, templateAtom, find, isUndef);
		//If exist and is fact then fail the search(find equal false) else if not exist
		// or is undefined atom then continue
		if(negation)
			find = !(find && !isUndef);
	}

	return true;
}

IndexAtom* ProgramGrounder::firstNextMatch( bool searchDelta, Instances* instance, bool& firstMatch,Atom*& templateAtom, bool& find) {
	// Otherwise a search is made in instance in order to provide bind variables with a value
	IndexAtom* indexingStrategy = instance->getIndex();
	// Determine if it is needed to perform a first or next match
	firstMatch = index_current_atom != current_id_match.size() - 1;
	if (templateAtom != nullptr)
		delete templateAtom;

	templateAtom = setBoundValue();
	// Perform a first match and save the integer identifier returned, useful to perform further next matches
	if (firstMatch) {
		unsigned int id = 0;
		id = indexingStrategy->firstMatch(searchDelta, templateAtom, current_var_assign, find);
		current_id_match.push_back(id);
	} else {
		removeBindValueInAssignment(current_variables_atoms[index_current_atom]);
		unsigned int id = current_id_match.back();
		indexingStrategy->nextMatch(id, current_var_assign, find);
	}
	return indexingStrategy;
}

bool ProgramGrounder::groundRule(Rule* r, bool firstIteraction, bool isRecursive, const unordered_set<index_object>* predicateInHead) {
	//The map of the assignment, map each variables to its assigned value
	currentRule=r;
	current_var_assign.clear();
	current_id_match.clear();


	if (r->getSizeBody() == 0)
		return evaluator.printGroundRule(instancesTable, predicateTable, statementDependency, r, current_var_assign, isRecursive, firstIteraction);


	//TODO Sort the atoms in the rule in a smarter way, currently no sorting is performed

	bool newKnowledge = false;
	bool finish = false;
	current_atom_it = currentRule->getBeginBody();
	index_current_atom = 0;
	bool find = false;
	bool negation = false;

	// Contain for each atom the set of variables
	Atom *templateAtom = 0;

	//Determine bind variables for each atoms
	findBindVariablesRule();

#if DEBUG == 1
	//DEBUG PRINT
	cout<<"--- INIZIALIZATION ---"<<endl;
	printVariables();
	cout<<"--- END INIZIALIZATION ---"<<endl;
	// END DEBUG PRINT
#endif

	while (!finish) {
		current_atom = *current_atom_it;
		Predicate* current_predicate = current_atom->getPredicate();
		negation = current_atom->isNegative();
		bool firstMatch;
		Instances * instance = instancesTable->getInstance(current_predicate);
		bool searchDelta = isRecursive && predicateInHead->count(current_predicate->getIndex()) && !firstIteraction;

		// If the current atom is a built in or the instance table is null
		// then firstMatch or nextMatch must not be invoked
		if (current_atom->isBuiltIn() || instance == nullptr || negation || ( current_variables_atoms[index_current_atom].size()==0 && !current_atom->containsAnonymous() )) {

			if (!groundBoundAtom( find, negation, searchDelta, instance, templateAtom))
				return false;

		} else {
			// Otherwise a search is made in instance in order to provide bind variables with a value

			firstNextMatch( searchDelta,  instance, firstMatch,  templateAtom, find);
		}

#if DEBUG == 1
		//DEBUG PRINT
		cout<<"ATOM "<<index_current_atom<<" ";
		current_atom->print();
		cout<<" --> ";
		Atom *literal=current_atom->ground(current_var_assign);
		literal->print();
		delete literal;
		if (find)
		cout<<" MATCH!"<<endl;
		else
		cout<<" NO-MATCH!"<<endl;
		// END DEBUG PRINT
#endif

		// If a match is found and the atom is not negated or
		// if no match is found, but the atom is negated, so it was performed first match on it
		// insert the bind variables returned values in the current assignment
		if (find) {

			//If there is no more atom, a valid assignment is found and it is printed
			if (index_current_atom + 1 == currentRule->getSizeBody()) {
				// The method printGroundRule returns true if the ground rule derived from the current assignment was not derived before,
				// in this case new knowledge is derived.
				if (evaluator.printGroundRule(instancesTable, predicateTable, statementDependency, currentRule, current_var_assign, isRecursive, firstIteraction))
					newKnowledge = true;

				//If last atom is BuiltIn return to last atom no BuiltIn
				skipAtom(false, finish);

#if DEBUG == 1
				//DEBUG PRINT
				cout<<" --> ";
				printAssignment();
				// END DEBUG PRINT
#endif

				//Otherwise the process continues with the next atom
			} else {
				current_atom_it++;
				index_current_atom++;
			}

			//Otherwise if there is no match
		} else {

			// If the process is come back to the first atom the grounding process is finished
			if (current_atom_it == currentRule->getBeginBody())
				finish = true;
			else {

				// Otherwise the current assignment is no valid anymore, and the grounding process
				// comes back to the previous atom skipping the BuiltIn
				skipAtom(true,  finish);

			}
		}
	}
	delete templateAtom;

	return newKnowledge;
}

void ProgramGrounder::printAssignment() {

	cout << "ASSIGNMENT ";
	for (auto i : current_var_assign) {
		cout << "[ ";
		i.first->print();
		cout << ",";
		i.second->print();
		cout << " ]";
	}
	cout << endl;

}

void ProgramGrounder::skipAtom(bool firstSkip, bool &finish) {
	if (firstSkip) {
		current_atom_it--;
		index_current_atom--;
		current_id_match.pop_back();
	};
	while ((*current_atom_it)->isBuiltIn() || (*current_atom_it)->isNegative() || current_variables_atoms[index_current_atom].size()==0) {
		if (current_atom_it == currentRule->getBeginBody()) {
			finish = true;
			break;
		}
		current_atom_it--;
		index_current_atom--;
		current_id_match.pop_back();
	}
}

void ProgramGrounder::printVariables() {
	unsigned int counter_atom_debug = 0;
	for (auto vec : current_variables_atoms) {
		cout << "Atom " << counter_atom_debug << " ";
		for (auto variable : vec)
			variable->print();
		cout << " ";
		cout << endl;
		counter_atom_debug++;
	}
}

void ProgramGrounder::findBindVariablesRule() {
	set_term total_variable;
	unsigned int index_current_atom = 0;
	current_variables_atoms.clear();

	//For each atom determines the bound and the bind variables
	for (auto current_atom_it = currentRule->getBeginBody(); current_atom_it != currentRule->getEndBody(); current_atom_it++) {
		Atom *current_atom = *current_atom_it;
		set_term variablesInAtom = current_atom->getVariable();
		current_variables_atoms.push_back(set_term());

		for (auto variable : variablesInAtom) {
			if (!total_variable.count(variable))
				current_variables_atoms[index_current_atom].insert(variable);
		}

		for (auto variable : variablesInAtom)
			total_variable.insert(variable);
		index_current_atom++;

	}
}

Atom* ProgramGrounder::setBoundValue() {
	return current_atom->ground(current_var_assign);
}

void ProgramGrounder::removeBindValueInAssignment(set_term bind_variables) {

	for (auto variable : bind_variables)
		current_var_assign.erase(variable);

}

ProgramGrounder::~ProgramGrounder() {
	delete predicateTable;
	delete instancesTable;
	delete statementDependency;
	delete termsMap;
}

