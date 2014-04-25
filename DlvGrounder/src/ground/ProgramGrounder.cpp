/*
 * ProgramGrounder.cpp
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#include "ProgramGrounder.h"

#include <list>
#include "../atom/ClassicalLiteral.h"

void ProgramGrounder::ground() {
	statementDependency->createDependencyGraph(predicateTable);
	statementDependency->createComponentGraph();

	//Ground first rule
//	groundJoinRule(statementDependency->getRule(0));
}

void ProgramGrounder::findBoundBindRule(Rule *r,vector<vec_pair_long> &bounds,vector<vec_pair_long>& binds){
	//variable,value
	unordered_map<unsigned long, unsigned long> var_assign;

	auto current_atom_it=r->getBeginBody();
	int index_current_atom=0;

	//INIZIALIZE bounds binds
	for(;current_atom_it!=r->getEndBody();current_atom_it++){
		Atom *current_atom=*current_atom_it;
		bounds.push_back(vec_pair_long());
		binds.push_back(vec_pair_long());
		for(unsigned int i=0;i<current_atom->getTerms().size();i++){
			auto f=var_assign.find(current_atom->getTerm(i));
			if(f!=var_assign.end()){
				bounds[index_current_atom].push_back({i,0});
			}else{
				// Bind only variable not anonymous
				if(!termsMap->getTerm(current_atom->getTerm(i))->isAnonymous())
					binds[index_current_atom].push_back({i,0});
			}

		}
		for(auto v:binds[index_current_atom])
			var_assign.insert({current_atom->getTerm(v.first),v.second});
		index_current_atom++;
	}
//
//	for(int i=0;i<index_current_atom;i++){
//		cout<<i<<" ";
//		cout<<"bound ";
//		for(int j=0;j<bounds[i].size();j++)
//			cout<<bounds[i][j].first<<" "<<bounds[i][j].second<<" ";
//		cout<<"bind ";
//		for(int j=0;j<binds[i].size();j++)
//			cout<<binds[i][j].first<<" ";
//		cout<<endl;
//	}
}

void ProgramGrounder::foundAssignmentRule(Rule *r,map_long_long& var_assign){

	vector<Atom*> atoms;
	for (auto head_it = r->getBeginHead(); head_it != r->getEndHead(); head_it++) {
		Atom *head=(*head_it);

		unsigned long predicate=head->getPredicate();
		vector<unsigned long> terms;

		for(unsigned int i=0;i<head->getTerms().size();i++){
			terms.push_back(var_assign.find(head->getTerm(i))->second);
		}

		atoms.push_back(new ClassicalLiteral(predicate,terms,false,false));

	}
	for(Atom *a:atoms){
		a->print(termsMap);
		cout<<"."<<endl;
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


void ProgramGrounder::getBindVariable(Atom *atom,vector<unsigned long>& bind){
	for(unsigned int i=0;i<atom->getTerms().size();i++)
		if(!termsMap->getTerm(atom->getTerm(i))->isAnonymous())
			bind.push_back(i);
}

void ProgramGrounder::getCommonVariablesIndex(Atom *a1,Atom *a2,vector<unsigned long>& common1,
		vector<unsigned long>& common2){
	for(unsigned int i=0;i<a1->getTerms().size();i++)
		for(unsigned int j=0;j<a2->getTerms().size();j++)
				if(!termsMap->getTerm(a1->getTerm(i))->isAnonymous() && a1->getTerm(i) == a2->getTerm(j) ){
					common1.push_back(i);
					common2.push_back(j);
				}
}


void ProgramGrounder::groundJoinRule(Rule* r) {

	Atom *atom1=*r->getBeginBody();
	Atom *atom2=*(r->getBeginBody()+1);

	vector<unsigned long> bind1,bind2,common1,common2;
	getBindVariable(atom1,bind1);
	getBindVariable(atom2,bind2);
	getCommonVariablesIndex(atom1,atom2,common1,common2);
	for(unsigned long i:common1)
		cout<<i<<" ";
	cout<<endl;
	for(unsigned long i:common2)
			cout<<i<<" ";
		cout<<endl;
	Hash_AtomSet hasher1(common1),hasher2(common2);
	Atom_Match_Set atom_set1(0,hasher1,hasher1),atom_set2(0,hasher2,hasher2);

	instancesTable->getInstance(atom1->getPredicate())->getIndex()->hashAtoms(atom_set1);
	instancesTable->getInstance(atom2->getPredicate())->getIndex()->hashAtoms(atom_set2);



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

	findBoundBindRule(r,bounds,binds);


	while(!finish){
		Atom *current_atom=*current_atom_it;
		IndexAtom *index=instancesTable->getInstance(current_atom->getPredicate())->getIndex();

		// Set value of bound based of the current assignment
		setBoundValue(current_atom,bounds[index_current_atom],var_assign);

		// FIND IF FIRST OR NEXT
		if(index_current_atom!=id_match.size()-1){

			unsigned long id=index->firstMatch(bounds[index_current_atom],binds[index_current_atom],find);
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

