/*
 * ProgramGrounder.cpp
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#include "ProgramGrounder.h"

#include <list>
#include "../atom/ClassicalLiteral.h"

#include "../utility/Timer.h"

void ProgramGrounder::ground() {
	statementDependency->createDependencyGraph(predicateTable);
	statementDependency->createComponentGraph();

	//Ground first rule
	Timer::getInstance()->start("Gound Rule");
	groundRule(statementDependency->getRule(0));
	Timer::getInstance()->end();
}

void ProgramGrounder::findBoundBindRule(Rule *r,vector<vec_pair_long> &bounds,vector<vec_pair_long>& binds,vector<map_int_int >& equal_vars){
	//variable in rule
	unordered_set<unsigned long> var_assign;
	//variable in atom
	unordered_map<unsigned long,unsigned int> var_atom;


	auto current_atom_it=r->getBeginBody();
	int index_current_atom=0;

	//INIZIALIZE bounds binds
	for(;current_atom_it!=r->getEndBody();current_atom_it++){
		Atom *current_atom=*current_atom_it;
		bounds.push_back(vec_pair_long());
		binds.push_back(vec_pair_long());
		equal_vars.push_back(unordered_multimap<unsigned int,unsigned int>());
		for(unsigned int i=0;i<current_atom->getTermsSize();i++){

			auto f=var_assign.find(current_atom->getTerm(i));
			if(f!=var_assign.end()){
				bounds[index_current_atom].push_back({i,0});
			}else{
				// Bind only variable not anonymous and initially contains the variable
				if(!termsMap->getTerm(current_atom->getTerm(i))->isAnonymous()){
					binds[index_current_atom].push_back({i,current_atom->getTerm(i)});

					auto it=var_atom.find(current_atom->getTerm(i));
					if(it!=var_atom.end())
							equal_vars[index_current_atom].insert({it->second,i});
						else
							//insert the variable and the index of term
							var_atom.insert({current_atom->getTerm(i),i});
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

void ProgramGrounder::printGroundRule(Rule *r,map_long_long& var_assign){
	//TODO
}

void ProgramGrounder::foundAssignmentRule(Rule *r,map_long_long& var_assign){

	vector<Atom*> atoms;
	for (auto head_it = r->getBeginHead(); head_it != r->getEndHead(); head_it++) {
		Atom *head=(*head_it);

		unsigned long predicate=head->getPredicate();
		vector<unsigned long> terms;

		for(unsigned int i=0;i<head->getTermsSize();i++){
			terms.push_back(var_assign.find(head->getTerm(i))->second);
		}

		atoms.push_back(new ClassicalLiteral(predicate,terms,false,false));

	}
	for(Atom *a:atoms){
		a->print(termsMap);
		cout<<".";
	}
}

void ProgramGrounder::setBoundValue(Atom *current_atom,vec_pair_long &bound,map_long_long& var_assign){
	for(unsigned int i=0;i<bound.size();i++){
		unsigned long bound_variable=current_atom->getTerm(bound[i].first);
		unsigned long bound_value=var_assign.find(bound_variable)->second;

		bound[i].second=bound_value;
	}
}

void ProgramGrounder::removeBindValueInAssignment(Atom *current_atom,vec_pair_long &bind,map_long_long& var_assign){
	for(auto v:bind){
		unsigned long bind_variable=current_atom->getTerm(v.first);
		var_assign.erase(bind_variable);
	}
}

void ProgramGrounder::insertBindValueInAssignment(Atom *current_atom,vec_pair_long &bind,map_long_long& var_assign){
	for(auto v:bind){
		unsigned long bind_variable=current_atom->getTerm(v.first);
		unsigned long bind_value=v.second;
		var_assign.insert({bind_variable,bind_value});
	}
}




void ProgramGrounder::groundRule(Rule* r) {
	//variable,value
	map_long_long var_assign;
	list<unsigned long> id_match(0);

	bool finish=false;
	auto current_atom_it=r->getBeginBody();
	unsigned int index_current_atom=0;
	bool find=false;

	vector<vec_pair_long> bounds,binds;
	vector<map_int_int > equal_vars;

	//TODO Order rule!
	findBoundBindRule(r,bounds,binds,equal_vars);

	while(!finish){
		Atom *current_atom=*current_atom_it;
		Instances * instance=instancesTable->getInstance(current_atom->getPredicate());

		// if there isn't instance of that atom exit of grounding
		if(instance==nullptr) return;

		IndexAtom *index=instance->getIndex();

		// Set value of bound based of the current assignment
		setBoundValue(current_atom,bounds[index_current_atom],var_assign);

		// FIND IF FIRST OR NEXT
		if(index_current_atom!=id_match.size()-1){

			unsigned long id=index->firstMatch(bounds[index_current_atom],binds[index_current_atom],equal_vars[index_current_atom],find);
			id_match.push_back(id);
		}else{
			unsigned long id=id_match.back();

			// Remove bind value in assignment
			removeBindValueInAssignment(current_atom,binds[index_current_atom],var_assign);

			index->nextMatch(id,binds[index_current_atom],find);


		}



		//IF MATCH
		if(find){


			// Insert bind variable assignment
			insertBindValueInAssignment(current_atom,binds[index_current_atom],var_assign);

			// IF END OF RULE
			if(current_atom_it+1==r->getEndBody()){

				foundAssignmentRule(r,var_assign);

			}else{

				current_atom_it++; index_current_atom++;

			}
		}else{


			//IF FAIL MATCH AND FIRST ATOM EXIT
			if(current_atom_it==r->getBeginBody())
				finish=true;

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

