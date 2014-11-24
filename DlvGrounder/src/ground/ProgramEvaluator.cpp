/*
 * ProgramEvaluator.cpp
 *
 *  Created on: Sep 7, 2014
 *      Author: david
 */

#include "ProgramEvaluator.h"

void ProgramEvaluator::printAndSimplify(InstancesTable* instancesTable) {
	if(!simplification)return ;

	bool skipRule = false;

	for(auto rule_it=rules.getBeginGroundOrder();rule_it!=rules.getEndGroundOrder();rule_it++){

		GroundRule *rule=*rule_it;
		skipRule = false;
		// For each atom in the body if is a fact and is negated then skip the rule and decrement the support
		// else the atom have to be deleted
		// If is undef and havo no support is false and if is negated delete the atom
		// if is true skip the rule and decrement the support
		vector<GroundAtom*> atomToRemove;
		for (auto body_it = rule->getBeginBody(); body_it != rule->getEndBody();
				body_it++) {
			GroundAtom* body = *body_it;
			Instances* is= instancesTable->getInstance(body->predicate);
			if (is!=nullptr && is->isTrue(body->atom->terms)) {
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

bool ProgramEvaluator::groundBody(bool disjunction, bool isRecursive,
		bool firstIteration, bool updated, Rule* r, map_term_term& var_assign,
		InstancesTable* instancesTable, GroundRule* groundRule, bool& added) {
	for (auto head_it = r->getBeginHead(); head_it != r->getEndHead();
			head_it++) {
		Atom* head = (*head_it);
		Atom* groundAtom = head->ground(var_assign);
		Predicate* predicate = groundAtom->getPredicate();
		vector<Term*> terms = groundAtom->getTerms();
		delete groundAtom;
		//Check if the atom is already grounded, if not it is added to no facts
		// If the atom in the head is grounded and is a fact the rule is not grounded
		instancesTable->addInstance(predicate);
		GroundAtom* headAtom;
		if ((!disjunction && groundRule->getSizeBody() == 0))
			headAtom = new GroundAtom(predicate, terms);
		else
			headAtom = new GroundAtom(predicate, terms, false);

		//Update its truth value FIXME in rule duplicate terms
		if (isRecursive) {
			bool add = false;
			if (firstIteration)
				add = instancesTable->getInstance(predicate)->addDelta(
						headAtom->atom, updated);
			else
				add = instancesTable->getInstance(predicate)->addNextDelta(
						headAtom->atom, updated);

			if (add)
				added = true;
		} else
			added = instancesTable->getInstance(predicate)->addNoFact(
					headAtom->atom, updated);

		// Duplication in head of rule
		if (!added && groundRule->findHead(headAtom))
			added = true;
		else
			groundRule->addInHead(headAtom);
	}
	return updated;
}

void ProgramEvaluator::groundHead(Rule* r, PredicateTable* predicateTable,
		map_term_term& var_assign, InstancesTable* instancesTable,
		StatementDependency* statementDep, GroundRule* groundRule) {
	//*********   Ground the body   *********
	for (auto body_it = r->getBeginBody(); body_it != r->getEndBody();
			body_it++) {
		Atom* body = (*body_it);
		Predicate* predicate = body->getPredicate();
		//If the predicate is EDB skip this atom
		if (body->isBuiltIn() || predicate->isEdb())
			continue;

		GenericAtom* atom = nullptr;
		Atom* groundAtom = body->ground(var_assign);
		vector<Term*> terms = groundAtom->getTerms();
		delete groundAtom;
		// If the atom is not negative then exist in instance
		// else the atom not exist and have to be created if is unstratified
		Instances* instance = instancesTable->getInstance(predicate);
		if (instance != nullptr)
			atom = instance->getGenericAtom(terms);

		if (atom == nullptr
				&& statementDep->isPredicateNegativeStratified(predicate->getIndex())
				&& body->isNegative())
			atom = new AtomUndef(terms, false);

		if (atom != nullptr && !atom->isFact())
			groundRule->addInBody(
					new GroundAtom(predicate, atom, body->isNegative()));
	}
}

void ProgramEvaluator::groundConstraint(Rule* r, PredicateTable* predicateTable, map_term_term& var_assign) {
	bool first=true;
	for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {
		Atom* body = (*body_it);
		Predicate* predicate = body->getPredicate();
		//If the predicate is EDB skip this atom
		if (predicate->isEdb() || body->isBuiltIn())
			continue;

		Atom* groundAtom = body->ground(var_assign);
		if(first)
			{cout<<":-";first=false;}
		else
			cout<<";";
		groundAtom->print();
		delete groundAtom;
	}
	cout <<"."<< endl;
}

bool ProgramEvaluator::printGroundRule(InstancesTable* instancesTable,PredicateTable *predicateTable,StatementDependency * statementDep,
			Rule *r, map_term_term& var_assign, bool isRecursive, bool firstIteration) {
	if(r->isAStrongConstraint() && !simplification){
		groundConstraint(r, predicateTable, var_assign);
		return false;
	}

	//Build a ground rule with the given vars assignment
	GroundRule *groundRule = new GroundRule;

	//*********   Ground the body   *********
	groundHead(r, predicateTable, var_assign, instancesTable, statementDep,groundRule);
	//If in the head of rule there is no disjunction, then it is added in the no facts as true
	bool disjunction = r->getSizeHead() > 1;



	//*********   Ground the atom in the head   *********
	bool added = false;
	bool updated = false;
	updated = groundBody(disjunction, isRecursive, firstIteration, updated, r,var_assign, instancesTable, groundRule, added);

	if (groundRule->getSizeHead() == 1 && groundRule->getSizeBody() == 0) { // If is a new fact
		// Duplication in head with disjunction and is fact
		GroundAtom* atom=*groundRule->getBeginHead();
		if(disjunction)
			instancesTable->getInstance(atom->predicate)->setValue(atom->atom->terms,true);
		if ((added && atom->atom->isFact()) || (!added && updated))
			groundRule->print();
		delete groundRule;
	} else{

		if(simplification)
			addRule(groundRule);
		else{
			groundRule->print();
			delete groundRule;
		}

	}

	return added;

}
