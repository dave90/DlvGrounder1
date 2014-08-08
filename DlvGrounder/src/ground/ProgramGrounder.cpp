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

void ProgramGrounder::printPair(int i, vector<vec_pair_index_object>& vec) {
	cout << "ATOM " << i << " ";
	for (auto v : vec[i]) {
		cout << " [" << v.first << ",";
		termsMap->getTerm(v.second)->print();
		cout << "] ";
	}
	cout<<endl;
}

void ProgramGrounder::printVecPair(string name,vector<vec_pair_index_object> &vec){

	cout<<name<<endl;
	for(unsigned int i=0;i<vec.size();i++){
		printPair(i, vec);
	}

}

void ProgramGrounder::printMapIntInt(string name,vector<map_int_int >& equal_vars){

	cout<<name<<endl;
	for(unsigned int i=0;i<equal_vars.size();i++){
		cout<<"ATOM "<<i<<" ";
		for(auto v:equal_vars[i])
			cout<< " ["<<v.first<<","<< v.second<<"] ";
		cout<<endl;
	}

}

void ProgramGrounder::printAssignment(map_index_object_index_object& var_assign){

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


void ProgramGrounder::findBoundBindRule(Rule *r,vector<vec_pair_index_object> &bounds,vector<vec_pair_index_object>& binds,vector<map_int_int >& equal_vars){
	//The set of variables in the rule
	unordered_set<index_object> var_assign;
	//This map is needed to determine equal variables, see later
	unordered_map<index_object,unsigned int> var_atom;

	auto current_atom_it=r->getBeginBody();
	int index_current_atom=0;

	//For each atom determines the bound and the bind variables, as well as equal variables
	for(;current_atom_it!=r->getEndBody();current_atom_it++){
		Atom *current_atom=*current_atom_it;
		// A vector of bound variables is a vec_pair_index_object, that is a vector of pairs <unsigned int, index_object>.
		// The integer represents the position in the terms vector in which the bound variable is,
		// while the index_object is the term itself. The same holds for bind variables.
		// The pairs are needed because some terms (anonymous variables, or equal vars) are skipped by the grounding process.
		bounds.push_back(vec_pair_index_object());
		binds.push_back(vec_pair_index_object());
		// A map of equal vars is a map whose keys and values are unsigned integers.
		// Indeed for each variable that appears more than one time,
		// it is stored in the map its position mapped to the position in which that variable appear first.
		equal_vars.push_back(map_int_int());

		for(unsigned int i=0;i<current_atom->getTermsSize();i++){

			auto f=var_assign.find(current_atom->getTerm(i).second);
			//If a term doesn't appear for the first time in the current atom or is not a variable it is classified as bound (FIXME add function)
			//The pair stored is made by the position, and the term itself.
			if(f!=var_assign.end() || !termsMap->getTerm(current_atom->getTerm(i).second)->isVariable()){
				bounds[index_current_atom].push_back({i,current_atom->getTerm(i).second});
			}else{
				// Otherwise if is not anonymous it is classified as bind. (FIXME add function)
				// The pair stored is made by the position, and the term itself. This is just for initialing purpose,
				// during the grounding process real possible values will be found.
				if(!termsMap->getTerm(current_atom->getTerm(i).second)->isAnonymous()){
					binds[index_current_atom].push_back({i,current_atom->getTerm(i).second});

					// In the var_atom map, are stored all the bind variables found.
					// If a variables is already present in var_atom, this means that it appears more than one time in the atom, so it is an equal var
					auto it=var_atom.find(current_atom->getTerm(i).second);
					if(it!=var_atom.end())
						equal_vars[index_current_atom].insert({it->second,i});
					else
						var_atom.insert({current_atom->getTerm(i).second,i});

				}
			}
		}
		// After computing the bind variables for the current atom, add them to the set of variables of the rule.
		// It is needed to put them just now, when they have been completely determined.
		// If they are put during the for loop, if there are equal variables, they are put in the bound variables,
		// so there will conflicts between bound and bind variables (their intersection is not empty).
		for(auto b: binds[index_current_atom])
			var_assign.insert(b.second);

		//Erase the content of var_atom, since it will be used for the next atom
		var_atom.clear();

		index_current_atom++;
	}
}

void ProgramGrounder::printGroundRule(Rule *r,map_index_object_index_object& var_assign){

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


void ProgramGrounder::setBoundValue(Atom *current_atom,vec_pair_index_object &bound,map_index_object_index_object& var_assign){
	for(unsigned int i=0;i<bound.size();i++){
		if(termsMap->getTerm(current_atom->getTerm(bound[i].first).second)->isVariable()){
			index_object bound_variable=current_atom->getTerm(bound[i].first).second;
			index_object bound_value=var_assign.find(bound_variable)->second;

			bound[i].second=bound_value;
		}
	}
}

void ProgramGrounder::removeBindValueInAssignment(Atom *current_atom,vec_pair_index_object &bind,map_index_object_index_object& var_assign){
	for(auto v:bind){
		index_object bind_variable=current_atom->getTerm(v.first).second;
		var_assign.erase(bind_variable);
	}
}

void ProgramGrounder::insertBindValueInAssignment(Atom *current_atom,vec_pair_index_object &bind,map_index_object_index_object& var_assign){
	for(auto v:bind){
		index_object bind_variable=current_atom->getTerm(v.first).second;
		index_object bind_value=v.second;
		var_assign.insert({bind_variable,bind_value});
	}
}



void ProgramGrounder::groundRule(Rule* r) {
	//The map of the assignment, map each variables to its assigned value
	map_index_object_index_object var_assign;
	list<unsigned int> id_match(0);

	//TODO Sort the atoms in the rule in a smarter way, currently no sorting is performed

	bool finish=false;
	auto current_atom_it=r->getBeginBody();
	unsigned int index_current_atom=0;
	bool find=false;
	bool negation=false;

	vector<vec_pair_index_object> bounds,binds;
	vector<map_int_int > equal_vars;

	//Determine bind, bound and equal variables
	findBoundBindRule(r,bounds,binds,equal_vars);

#if DEBUG == 1
	//DEBUG PRINT
	cout<<"--- INIZIALIZATION ---"<<endl;
	printVecPair("BOUND",bounds);
	printVecPair("BIND",binds);
	printMapIntInt("EQUAL",equal_vars);
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

			//According to the current assignment, set the values of the bound variables of the current atom
			setBoundValue(current_atom,bounds[index_current_atom],var_assign);

			// Finally the bind variables are provided with a value

			// Determine if it is needed to perform a first or next match
			firstMatch=index_current_atom!=id_match.size()-1;

			//Perform a first match and save the integer identifier returned, useful to perform further next matches
			if(firstMatch){
				unsigned int id=indexingStrategy->firstMatch(bounds[index_current_atom],binds[index_current_atom],equal_vars[index_current_atom],find);
				id_match.push_back(id);

			}else{
				//If the atom is not negated perform a next match
				if(!negation){
					unsigned int id=id_match.back();

					// Remove bind value in assignment
					removeBindValueInAssignment(current_atom,binds[index_current_atom],var_assign);

					indexingStrategy->nextMatch(id,binds[index_current_atom],find);
				}

			}

		}


#if DEBUG == 1
		//DEBUG PRINT
		cout<<"ATOM "<<index_current_atom<<" ";
		current_atom->print();
		cout<<" --> ";
		vector<index_object> terms=current_atom->getTerms();
		for(auto bind:binds[index_current_atom])
			terms[bind.first]=bind.second;
		for(auto bound:bounds[index_current_atom])
			terms[bound.first]=bound.second;
		ClassicalLiteral literal(current_atom->getPredicate().second,terms,false,false);
		literal.print();
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
			insertBindValueInAssignment(current_atom,binds[index_current_atom],var_assign);

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

