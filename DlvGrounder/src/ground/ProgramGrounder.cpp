/*
 * ProgramGrounder.cpp
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */


#define DEBUG 1


#include "ProgramGrounder.h"

#include <list>
#include "../atom/ClassicalLiteral.h"

#include "../utility/Timer.h"


void ProgramGrounder::ground() {
	statementDependency->createDependencyGraph(predicateTable);
	statementDependency->createComponentGraph();

	//Ground first rule
	printFact();
	Timer::getInstance()->start("Gound Rule");
	for(unsigned int i=0;i<statementDependency->getRulesSize();i++)
		groundRule(statementDependency->getRule(i));
	Timer::getInstance()->end();
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
	//variable in rule
	unordered_set<index_object> var_assign;
	//variable in atom
	unordered_map<index_object,unsigned int> var_atom;


	auto current_atom_it=r->getBeginBody();
	int index_current_atom=0;

	//INIZIALIZE bounds binds
	for(;current_atom_it!=r->getEndBody();current_atom_it++){
		Atom *current_atom=*current_atom_it;
		bounds.push_back(vec_pair_index_object());
		binds.push_back(vec_pair_index_object());
		equal_vars.push_back(unordered_multimap<unsigned int,unsigned int>());
		for(unsigned int i=0;i<current_atom->getTermsSize();i++){

			auto f=var_assign.find(current_atom->getTerm(i).second);
			// if is bound or is a constant FIXME with function
			if(f!=var_assign.end() || !termsMap->getTerm(current_atom->getTerm(i).second)->isVariable()){
				bounds[index_current_atom].push_back({i,current_atom->getTerm(i).second});
			}else{
				// Bind only variable not anonymous and initially contains the variable
				if(!termsMap->getTerm(current_atom->getTerm(i).second)->isAnonymous()){
					binds[index_current_atom].push_back({i,current_atom->getTerm(i).second});

					auto it=var_atom.find(current_atom->getTerm(i).second);
					if(it!=var_atom.end())
							equal_vars[index_current_atom].insert({it->second,i});
						else
							//insert the variable and the index of term
							var_atom.insert({current_atom->getTerm(i).second,i});
				}
			}
		}
		// Put in assignment the bind variable (put now because if exist repetition variable conflict bind bound)
		for(auto b: binds[index_current_atom])
			var_assign.insert(b.second);

		var_atom.clear();

		index_current_atom++;
	}


}

void ProgramGrounder::printGroundRule(Rule *r,map_index_object_index_object& var_assign){

	GroundRule *groundRule=new GroundRule;

	//Add in no fact with true if no disjunction
	bool isTrue=r->getSizeHead()==0;

	//Groung atom in head
	for (auto head_it = r->getBeginHead(); head_it != r->getEndHead(); head_it++) {
		Atom *head=(*head_it);

		index_object predicate=head->getPredicate().second;
		vector<index_object> terms=head->getTerms();

		for(unsigned int i=0;i<head->getTermsSize();i++){
			if(var_assign.count(head->getTerm(i).second))
				terms[i]=var_assign[head->getTerm(i).second];
			else
				terms[i]=head->getTerm(i).second;
		}

		GroundAtom *headAtom=new GroundAtom(predicate,terms);
		//check if atom is already grounded
		instancesTable->addInstance(predicate);
		//FIXME in rule duplicate terms
		instancesTable->getInstance(predicate)->addNoFact(headAtom->atom,isTrue);

		groundRule->addInHead(headAtom);

	}

	if(r->isAStrongConstraint()){
		//Ground body, consider only Classical Literal
		for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {

			Atom *body=(*body_it);

			index_object predicate=body->getPredicate().second;
			//If the predicate is Edb jump this atom
			if(predicateTable->getPredicate(predicate)->isEdb())continue;

			vector<index_object> terms;

			//FIXME consider the anonymus variable
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
	//variable,value
	map_index_object_index_object var_assign;
	list<unsigned int> id_match(0);

	//TODO Order rule!
	bool finish=false;
	auto current_atom_it=r->getBeginBody();
	unsigned int index_current_atom=0;
	bool find=false;
	bool negation=false;

	vector<vec_pair_index_object> bounds,binds;
	vector<map_int_int > equal_vars;

	findBoundBindRule(r,bounds,binds,equal_vars);

#if DEBUG == 0
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

		Instances * instance=instancesTable->getInstance(current_atom->getPredicate().second);

		// if there isn't instance of that atom exit of grounding
		if(instance==nullptr) return;

		IndexAtom *index=instance->getIndex();

		// Set value of bound based of the current assignment
		setBoundValue(current_atom,bounds[index_current_atom],var_assign);


		// FIND IF FIRST OR NEXT
		bool firstMatch=index_current_atom!=id_match.size()-1;


		if(firstMatch){
			unsigned int id=index->firstMatch(bounds[index_current_atom],binds[index_current_atom],equal_vars[index_current_atom],find);
			id_match.push_back(id);

		}else{
			if(!negation){
				unsigned int id=id_match.back();

				// Remove bind value in assignment
				removeBindValueInAssignment(current_atom,binds[index_current_atom],var_assign);

				index->nextMatch(id,binds[index_current_atom],find);
			}

		}

#if DEBUG == 0
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

		//IF MATCH
		if( (find && !negation) | (!find && negation && firstMatch)){


			// Insert bind variable assignment
			insertBindValueInAssignment(current_atom,binds[index_current_atom],var_assign);


			// IF END OF RULE
			if(index_current_atom+1==r->getSizeBody()){

				printGroundRule(r,var_assign);
#if DEBUG == 0
			//DEBUG PRINT
			cout<<" --> ";
			printAssignment(var_assign);
			// END DEBUG PRINT
#endif

			}else{

				current_atom_it++; index_current_atom++;

			}
		}else{


			//IF FAIL MATCH AND FIRST ATOM EXIT
			if(current_atom_it==r->getBeginBody()){
				finish=true;
			}

			current_atom_it--;index_current_atom--;
			id_match.pop_back();

		}

	}


}

ProgramGrounder::~ProgramGrounder() {
	delete predicateTable;
	delete instancesTable;
	delete statementDependency;
	delete termsMap;

}

