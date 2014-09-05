/*
 * ProgramGrounder.cpp
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#define DEBUG 1

#include <list>

#include "ProgramGrounder.h"
#include "../atom/ClassicalLiteral.h"
#include "../utility/Timer.h"

void GroundedRules::printAndSimplify(InstancesTable* instancesTable) {
	bool skipRule = false;

	for (GroundRule* rule : groundedRulesOrdering) {
		skipRule = false;
		// For each atom in the body if is a fact and is negated then skip the rule and decrement the support
		// else the atom have to be deleted
		// If is undef and havo no support is false and if is negated delete the atom
		// if is true skip the rule and decrement the support
		vector<GroundAtom*> atomToRemove;
		for (auto body_it = rule->getBeginBody(); body_it != rule->getEndBody();
				body_it++) {
			GroundAtom* body = *body_it;
			if (instancesTable->getInstance(body->predicate)->isTrue(body->atom->terms)) {
				if (body->negative) {
					decrementSupport(rule);
					skipRule = true;
					break;
				} else
					atomToRemove.push_back(body);
			} else {
				auto it=supportedAtom.find(body);
				if (it == supportedAtom.end() || it->second == 0) {
					if (!body->negative) {
						decrementSupport(rule);
						skipRule = true;
						break;
					} else
						atomToRemove.push_back(body);
				}
			}
		}

		if (skipRule)
			continue;
		for (auto atom : atomToRemove)
			rule->removeBody(atom);

		// For each atom in the head if is 1 and body is empty is a fact
		// else if is a fact then skip the rule and decrement support
		if (rule->getSizeHead() == 1 && rule->getSizeBody() == 0) {
			GroundAtom* head = *rule->getBeginHead();
			instancesTable->getInstance(head->predicate)->setValue(
					head->atom->terms, true);
			rule->print();
			continue;
		}
		for (auto head_it = rule->getBeginHead(); head_it != rule->getEndHead();head_it++) {
			GroundAtom* head = *head_it;

			if (instancesTable->getInstance(head->predicate)->isTrue(head->atom->terms)) {
				decrementSupport(rule);
				skipRule = true;
				break;
			}
		}

		if (!skipRule)
			rule->print();

	}
}
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
	statementDependency->createComponentGraphAndComputeAnOrdering(exitRules,
			recursiveRules, componentPredicateInHead);

	printFact();

	Timer::getInstance()->start("Ground Rule");
	// Ground each module according to the ordering:
	// For each component, each rule is either recursive or exit,
	// Exit rules are grounded just once, while recursive rules are grounded until no more knowledge is derived
	for (unsigned int component = 0; component < exitRules.size();
			component++) {

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
						if (groundRule(r, firstIteration, true,
								&componentPredicateInHead[component]))
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
			groundRule(statementDependency->getConstraint(i), false, false,
					nullptr);

	//Print and simplify the rule
	groundedRule.printAndSimplify(instancesTable);

	Timer::getInstance()->end();
}

void ProgramGrounder::updateDelta(Rule* r) {
	for (auto it = r->getBeginHead(); it != r->getEndHead(); it++) {
		index_object predicate = (*it)->getPredicate().second;
		Instances* is = instancesTable->getInstance(predicate);
		if(is!=nullptr) is->moveNextDeltaInDelta();
	}
}

bool ProgramGrounder::groundRule(Rule* r, bool firstIteraction,
		bool isRecursive, const unordered_set<index_object>* predicateInHead) {
	//The map of the assignment, map each variables to its assigned value
	map_index_index var_assign;
	if (r->getSizeBody() == 0)
		return printGroundRule(r, var_assign, isRecursive, firstIteraction);

	list<unsigned int> id_match(0);

	//TODO Sort the atoms in the rule in a smarter way, currently no sorting is performed

	bool newKnowledge = false;

	bool finish = false;
	auto current_atom_it = r->getBeginBody();

	unsigned int index_current_atom = 0;
	bool find = false;
	bool negation = false;

	// Contain for each atom the set of variables
	vector<unordered_set<index_object>> variables_atoms;
	Atom *templateAtom = 0;

	//Determine bind variables for each atoms
	findBindVariablesRule(r, variables_atoms);

#if DEBUG == 1
	//DEBUG PRINT
	cout<<"--- INIZIALIZATION ---"<<endl;
	printVariables(variables_atoms);
	cout<<"--- END INIZIALIZATION ---"<<endl;
	// END DEBUG PRINT
#endif

	while (!finish) {
		Atom *current_atom = *current_atom_it;
		index_object current_predicate = current_atom->getPredicate().second;
		negation = current_atom->isNegative();
		bool firstMatch;
		Instances * instance = instancesTable->getInstance(current_predicate);
		bool searchDelta = isRecursive
				&& predicateInHead->count(current_predicate)
				&& !firstIteraction;

		// If the current atom is a built in or the instance table is null
		// then firstMatch or nextMatch must not be invoked
		if (current_atom->isBuiltIn() || instance == nullptr || negation) {

			firstMatch = true;

			id_match.push_back(0);

			//  If it is a built in atom, ground it and evaluate it (Built in have not instance table, since they have not a predicate)
			if (current_atom->isBuiltIn()) {

				Atom *groundBuiltIn = current_atom->ground(var_assign);
				find = groundBuiltIn->evaluate();
				delete groundBuiltIn;

				// If there isn't instances the search fails (find equal false) and if isn't negated then the algorithm have to stop
			} else if (instance == nullptr) {
				if (!negation)
					return false;
				else
					find = true;

			} else if (negation) {

				if (templateAtom != nullptr)
					delete templateAtom;
				templateAtom = setBoundValue(current_atom, var_assign);
				bool isUndef;

				instance->getIndex()->findIfExist(searchDelta, templateAtom,
						find, isUndef);
				//If exist and is fact then fail the search(find equal false) else if not exist
				// or is undefined atom then continue
				find = !(find && !isUndef);

			}

		} else {
			// Otherwise a search is made in instance in order to provide bind variables with a value
			IndexAtom* indexingStrategy = instance->getIndex();

			// Determine if it is needed to perform a first or next match
			firstMatch = index_current_atom != id_match.size() - 1;

			// Remove bind value in assignment (from the previous assignment)
			removeBindValueInAssignment(variables_atoms[index_current_atom],
					var_assign);
			if (templateAtom != nullptr)
				delete templateAtom;
			templateAtom = setBoundValue(current_atom, var_assign);

			// Perform a first match and save the integer identifier returned, useful to perform further next matches
			if (firstMatch) {
				unsigned int id = 0;

				id = indexingStrategy->firstMatch(searchDelta, templateAtom,
						var_assign, find);
				id_match.push_back(id);

			} else {
				unsigned int id = id_match.back();
				indexingStrategy->nextMatch(id, templateAtom, var_assign, find);
			}

		}

#if DEBUG == 1
		//DEBUG PRINT
		cout<<"ATOM "<<index_current_atom<<" ";
		current_atom->print();
		cout<<" --> ";
		Atom *literal=current_atom->ground(var_assign);
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
			if (index_current_atom + 1 == r->getSizeBody()) {
				// The method printGroundRule returns true if the ground rule derived from the current assignment was not derived before,
				// in this case new knowledge is derived.
				if (printGroundRule(r, var_assign, isRecursive,
						firstIteraction))
					newKnowledge = true;

				//If last atom is BuiltIn return to last atom no BuiltIn
				skipAtom(false, current_atom_it, index_current_atom, id_match,
						r, finish);

#if DEBUG == 1
				//DEBUG PRINT
				cout<<" --> ";
				printAssignment(var_assign);
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
			if (current_atom_it == r->getBeginBody())
				finish = true;
			else {

				// Otherwise the current assignment is no valid anymore, and the grounding process
				// comes back to the previous atom skipping the BuiltIn
				skipAtom(true, current_atom_it, index_current_atom, id_match, r,
						finish);

			}
		}
	}
	delete templateAtom;

	return newKnowledge;
}

void ProgramGrounder::printAssignment(map_index_index& var_assign) {

	cout << "ASSIGNMENT ";
	for (auto i : var_assign) {
		cout << "[ ";
		termsMap->getTerm(i.first)->print();
		cout << ",";
		termsMap->getTerm(i.second)->print();
		cout << " ]";
	}
	cout << endl;

}

void ProgramGrounder::skipAtom(bool firstSkip,
		vector<Atom*>::const_iterator & current_atom_it,
		unsigned int &index_current_atom, list<unsigned int> &id_match, Rule *r,
		bool &finish) {
	if (firstSkip) {
		current_atom_it--;
		index_current_atom--;
		id_match.pop_back();
	};
	while ((*current_atom_it)->isBuiltIn() || (*current_atom_it)->isNegative()) {
		if (current_atom_it == r->getBeginBody()) {
			finish = true;
			break;
		}
		current_atom_it--;
		index_current_atom--;
		id_match.pop_back();
	}
}

void ProgramGrounder::printVariables(
		vector<unordered_set<index_object>> variables_atoms) {
	unsigned int counter_atom_debug = 0;
	for (auto vec : variables_atoms) {
		cout << "Atom " << counter_atom_debug << " ";
		for (auto variable : vec)
			termsMap->getTerm(variable)->print();
		cout << " ";
		cout << endl;
		counter_atom_debug++;
	}
}

void ProgramGrounder::findBindVariablesRule(Rule *r,
		vector<unordered_set<index_object> >& variables) {
	unordered_set<index_object> total_variable;
	unsigned int index_current_atom = 0;

	//For each atom determines the bound and the bind variables
	for (auto current_atom_it = r->getBeginBody();
			current_atom_it != r->getEndBody(); current_atom_it++) {
		Atom *current_atom = *current_atom_it;
		unordered_set<index_object> variablesInAtom =
				current_atom->getVariable();
		variables.push_back(unordered_set<index_object>());

		for (auto variable : variablesInAtom) {
			if (!total_variable.count(variable))
				variables[index_current_atom].insert(variable);
		}

		for (auto variable : variablesInAtom)
			total_variable.insert(variable);
		index_current_atom++;

	}
}

bool ProgramGrounder::printGroundRule(Rule *r, map_index_index& var_assign,
		bool isRecursive, bool firstIteration) {
	//Build a ground rule with the given vars assignment
	GroundRule *groundRule = new GroundRule;

	//Ground the body
	for (auto body_it = r->getBeginBody(); body_it != r->getEndBody();body_it++) {

		Atom *body = (*body_it);

		index_object predicate = body->getPredicate().second;
		//If the predicate is EDB skip this atom
		if (predicateTable->getPredicate(predicate)->isEdb())
			continue;

		GenericAtom *atom = nullptr;
		Atom *groundAtom = body->ground(var_assign);
		vector<index_object> terms = groundAtom->getTerms();
		delete groundAtom;

		// If the atom is not negative then exist in instance
		// else the atom not exist and have to be created if is unstratified

		Instances *instance = instancesTable->getInstance(predicate);
		if (instance != nullptr)
			atom = instance->getGenericAtom(terms);

		if (atom == nullptr
				&& statementDependency->isPredicateNegativeStratified(predicate)
				&& body->isNegative())
			atom = new AtomUndef(terms, false);

		if (atom != nullptr && !atom->isFact())
			groundRule->addInBody(
					new GroundAtom(predicate, atom, body->isNegative()));

	}
	//If in the head of rule there is no disjunction, then it is added in the no facts as true
	bool disjunction = r->getSizeHead() > 1;

	bool added = false;
	//Ground the atom in the head
	for (auto head_it = r->getBeginHead(); head_it != r->getEndHead();head_it++) {
		Atom *head = (*head_it);

		Atom *groundAtom = head->ground(var_assign);
		index_object predicate = groundAtom->getPredicate().second;
		vector<index_object> terms = groundAtom->getTerms();
		delete groundAtom;

		//Check if the atom is already grounded, if not it is added to no facts
		// If the atom in the head is grounded and is a fact the rule is not grounded
		instancesTable->addInstance(predicate);

		GroundAtom *headAtom;

		if ((!disjunction && groundRule->getSizeBody() == 0 ) )
			headAtom = new GroundAtom(predicate, terms);
		else
			headAtom = new GroundAtom(predicate, terms, false);

		//Update its truth value FIXME in rule duplicate terms
		if (isRecursive) {
			bool add = false;
			if (firstIteration)
				add = instancesTable->getInstance(predicate)->addDelta(
						headAtom->atom);
			else
				add = instancesTable->getInstance(predicate)->addNextDelta(
						headAtom->atom);
			if (add)
				added = true;
		} else
			added = instancesTable->getInstance(predicate)->addNoFact(
					headAtom->atom);

		groundRule->addInHead(headAtom);

	}

	if (groundRule->getSizeHead() == 1 && groundRule->getSizeBody() == 0) { // If is a new fact
		if (added) {
			groundRule->print();
		}
		delete groundRule;
	} else
		groundedRule.addRule(groundRule);

	return added;

}

Atom* ProgramGrounder::setBoundValue(Atom *current_atom,
		map_index_index& var_assign) {
	return current_atom->ground(var_assign);
}

void ProgramGrounder::removeBindValueInAssignment(
		unordered_set<index_object> bind_variables,
		map_index_index& var_assign) {

	for (index_object variable : bind_variables)
		var_assign.erase(variable);

}

ProgramGrounder::~ProgramGrounder() {
	delete predicateTable;
	delete instancesTable;
	delete statementDependency;
	delete termsMap;

}

