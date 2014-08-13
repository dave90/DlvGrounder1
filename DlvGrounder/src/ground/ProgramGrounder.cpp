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


void ProgramGrounder::ground() {

	//Create the dependency graph
	statementDependency->createDependencyGraph(predicateTable);

	// Create the component graph and compute an ordering among components
	// Components' rules are classified as exit or recursive.
	// An rule occurring in a component is recursive if there is a predicate belonging
	// to the component in its positive body, otherwise it is said to be an exit rule.
	vector<vector<Rule*>> exitRules;
	vector<vector<Rule*>> recursiveRules;
	statementDependency->createComponentGraphAndComputeAnOrdering(exitRules,recursiveRules);

	printFact();

	Timer::getInstance()->start("Ground Rule");
	//If there are more than one component, ground them according to the ordering
	if(exitRules.size()>1)
	{
		for(unsigned int component=0;component<exitRules.size();component++){

			/// Grounding of exit rules
			for(Rule* r: exitRules[component])
				groundRule(r);

			/// Grounding of recursive rules
			bool finish=false;
			while(!finish && recursiveRules[component].size()>0){
				for(Rule* r: recursiveRules[component])
					if(groundRecursiveRule(r))
						finish=true;
			}

		}
	}
	//If there aren't components, then there is just an EDB predicate.
	//In this case simply ground the rules defining it.
	else{
		for(unsigned int i=0;i<statementDependency->getRulesSize();i++)
			groundRule(statementDependency->getRule(i));
	}
	Timer::getInstance()->end();
}

bool ProgramGrounder::groundRecursiveRule(Rule* r){
	//TODO
	return 1;
}


void ProgramGrounder::printAssignment(map_index_index& var_assign){

	cout<<"ASSIGNMENT ";
	for(auto i:var_assign){
		cout<<"[ ";
		termsMap->getTerm(i.first)->print();
		cout<<",";
		termsMap->getTerm(i.second)->print();
		cout<<" ]";
	}
	cout<<endl;

}

void ProgramGrounder::printVariables(vector<unordered_set<index_object>> variables_atoms){
	unsigned int counter_atom_debug=0;
	for(auto vec:variables_atoms){
		cout<<"Atom "<<counter_atom_debug<<" ";
		for(auto variable:vec)
			termsMap->getTerm(variable)->print();cout<<" ";
		cout<<endl;
		counter_atom_debug++;
	}
}


void ProgramGrounder::findBindVariablesRule(Rule *r,vector<unordered_set<index_object> >& variables){
	unordered_set<index_object> total_variable;
	unsigned int index_current_atom=0;

	//For each atom determines the bound and the bind variables
	for(auto current_atom_it=r->getBeginBody();current_atom_it!=r->getEndBody();current_atom_it++){
		Atom *current_atom=*current_atom_it;
		unordered_set<index_object> variablesInAtom=current_atom->getVariable();
		variables.push_back(unordered_set<index_object>());

		for(auto variable:variablesInAtom){
			if(!total_variable.count(variable))
				variables[index_current_atom].insert(variable);
		}

		for(auto variable:variablesInAtom)total_variable.insert(variable);
		index_current_atom++;

	}
}

void ProgramGrounder::printGroundRule(Rule *r,map_index_index& var_assign){

	//Build a ground rule with the given vars assignment
	GroundRule *groundRule=new GroundRule;

	//If in the head of rule there is no disjunction, then it is added in the no facts as true
	bool isTrue = r->getSizeHead()==0;

	//Ground the atom in the head
	for (auto head_it = r->getBeginHead(); head_it != r->getEndHead(); head_it++) {
		Atom *head=(*head_it);

		Atom *groundAtom=head->ground(var_assign);
		index_object predicate=groundAtom->getPredicate().second;
		vector<index_object> terms=groundAtom->getTerms();


		GroundAtom *headAtom=new GroundAtom(predicate,terms);
		//Check if the atom is already grounded, if not it is added to no facts
		instancesTable->addInstance(predicate);
		//Update its truth value FIXME in rule duplicate terms
		instancesTable->getInstance(predicate)->addNoFact(headAtom->atom,isTrue);

		groundRule->addInHead(headAtom);
		delete groundAtom;

	}

	if(r->isAStrongConstraint()){

		//Ground the body if it is a strong constraint, FIXME currently just classical literals are considered
		for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {

			Atom *body=(*body_it);

			index_object predicate=body->getPredicate().second;
			//If the predicate is EDB skip this atom
			if(predicateTable->getPredicate(predicate)->isEdb()) continue;

			vector<index_object> terms;

			//FIXME consider the anonymous variable
			for(unsigned int i=0;i<body->getTermsSize();i++){


				index_object term=body->getTerm(i).second;

				if(termsMap->getTerm(term)->isVariable())
					terms.push_back(var_assign.find(term)->second);
				else
					terms.push_back(term);
			}

			GenericAtom *atom=instancesTable->getInstance(predicate)->getGenericAtom(terms);

			//FIXME duplicate fact
			groundRule->addInBody(new GroundAtom(predicate,atom));
		}
	}

	if(groundedRule.addRule(groundRule))
		groundRule->print();
}


Atom* ProgramGrounder::setBoundValue(Atom *current_atom,map_index_index& var_assign){
	return current_atom->ground(var_assign);
}

void ProgramGrounder::removeBindValueInAssignment(unordered_set<index_object> bind_variables,map_index_index& var_assign){

	for(index_object variable:bind_variables)
		var_assign.erase(variable);

}


void ProgramGrounder::groundRule(Rule* r) {
	//The map of the assignment, map each variables to its assigned value
	map_index_index var_assign;
	list<unsigned int> id_match(0);

	//TODO Sort the atoms in the rule in a smarter way, currently no sorting is performed

	bool finish=false;
	auto current_atom_it=r->getBeginBody();
	unsigned int index_current_atom=0;
	bool find=false;
	bool negation=false;

	// Contain for each atom the set of variables
	vector<unordered_set<index_object> > variables_atoms;
	Atom *templateAtom=0;

	//Determine bind value for each atoms
	findBindVariablesRule(r,variables_atoms);

#if DEBUG == 1
	//DEBUG PRINT
	cout<<"--- INIZIALIZATION ---"<<endl;
	printVariables(variables_atoms);
	cout<<"--- END INIZIALIZATION ---"<<endl;
	// END DEBUG PRINT
#endif

	while(!finish){
		Atom *current_atom=*current_atom_it;
		negation=current_atom->isNegative();
		bool firstMatch;
		Instances * instance=instancesTable->getInstance(current_atom->getPredicate().second);

		// Special case where must not call firstMatch or nextMatch
		if(current_atom->isBuiltIn() || instance==nullptr){
			// If is builtIn ground builtIn and evaluate
			// If there isn't instances the search fails (find equal false) and if isn't negated then the algoritm have to stop


			firstMatch=true;
			id_match.push_back(0);

			// First check builtin because haven't an Instances
			if(current_atom->isBuiltIn()){

				Atom *groundBuiltIn=current_atom->ground(var_assign);
				find=groundBuiltIn->evaluate();
				delete groundBuiltIn;

			}else if(instance==nullptr){

				if(!negation)
					return ;
				else
					find=false;

			}

		}else{


			IndexAtom* indexingStrategy = instance->getIndex();


			// Finally the bind variables are provided with a value

			// Determine if it is needed to perform a first or next match
			firstMatch=index_current_atom!=id_match.size()-1;

			//According to the current assignment, set the values of the bound variables of the current atom
			// Remove bind value in assignment
			removeBindValueInAssignment(variables_atoms[index_current_atom],var_assign);
			if(templateAtom!=nullptr)delete templateAtom;
			templateAtom=setBoundValue(current_atom,var_assign);

			//Perform a first match and save the integer identifier returned, useful to perform further next matches
			if(firstMatch){

				unsigned int id=indexingStrategy->firstMatch(templateAtom,var_assign,find);
				id_match.push_back(id);


			}else{
				//If the atom is not negated perform a next match
				if(!negation){
					unsigned int id=id_match.back();
					indexingStrategy->nextMatch(id,templateAtom,var_assign,find);
				}

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
		if( (find && !negation) | (!find && negation && firstMatch))
			cout<<" MATCH!"<<endl;
		else
			cout<<" NO-MATCH!"<<endl;
		// END DEBUG PRINT
#endif

		// If a match is found and the atom is not negated or
		// if no match is found, but the atom is negated, so it was performed first match on it
		// insert the bind variables returned values in the current assignment
		if( (find && !negation) || (!find && negation && firstMatch)){

			//If there is no more atom, a valid assignment is found and it is printed
			if(index_current_atom+1==r->getSizeBody()){
				printGroundRule(r,var_assign);

				//If last atom is BuiltIn return to last atom no BuiltIn
				while((*current_atom_it)->isBuiltIn() || (*current_atom_it)->isNegative()){current_atom_it--;index_current_atom--;id_match.pop_back();}

#if DEBUG == 1
			//DEBUG PRINT
			cout<<" --> ";
			printAssignment(var_assign);
			// END DEBUG PRINT
#endif

			//Otherwise the process continues with the next atom
			}else{
				current_atom_it++; index_current_atom++;
			}

		//Otherwise if there is no match
		}else{

			// If the process is come back to the first atom the grounding process is finished
			if(current_atom_it==r->getBeginBody())
				finish=true;
			else{

				// Otherwise the current assignment is no valid anymore, and the grounding process
				// comes back to the previous atom skipping the BuiltIn
				do{current_atom_it--;index_current_atom--;id_match.pop_back();}while((*current_atom_it)->isBuiltIn() || (*current_atom_it)->isNegative());

			}
		}

	}

}

ProgramGrounder::~ProgramGrounder() {
	delete predicateTable;
	delete instancesTable;
	delete statementDependency;
	delete termsMap;

}

