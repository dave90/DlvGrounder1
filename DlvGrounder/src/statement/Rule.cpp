/*
 * Rule.cpp
 *
 *  Created on: 03/apr/2014
 *      Author: Jessica
 */

#include "Rule.h"
#include "iostream"

set_predicate Rule::calculatePredicate(vector<Atom*>& atoms,bool checkNegative,bool negative){
	set_predicate predicates;
	for (auto atom:atoms) {
		Predicate* predicate=atom->getPredicate();
		if(predicate!=nullptr)
			if(!checkNegative || (atom->isNegative() == negative))
				predicates.insert(predicate);

	}
	return predicates;
}

unordered_set<index_object> Rule::calculatePredicateIndex(vector<Atom*>& atoms,bool checkNegative,bool negative){
	unordered_set<index_object> predicates;
	for (auto atom:atoms) {
		Predicate* predicate=atom->getPredicate();
		if(predicate!=nullptr)
			if(!checkNegative || (atom->isNegative() == negative))
				predicates.insert(predicate->getIndex());

	}
	return predicates;
}


void  Rule::print(){
	unsigned int i=0;
	for (auto atom:head) {
		atom->print();
		if(i!=head.size()-1)
			cout<<";";
		i++;
	}
	if(body.size()>0){
		cout<<":-";
		unsigned int i=0;
		for (auto atom:body) {
			atom->print();
			if(i!=body.size()-1)
				cout<<";";
			i++;
		}
	}
	cout<<"."<<endl;
}

bool insertVariables(Atom* atom,set_term& variables,bool anonymusSafe)
{
	if(!anonymusSafe)
		if(atom->containsAnonymous())
			return false;
	set_term tempVariables=atom->getVariable();
	variables.insert(tempVariables.begin(),tempVariables.end());
	return true;

}

bool Rule::isSafe()
{
	set_term variableToCheck;
	set_term variableInPositiveAtom;
	for(auto atom:head)
		if(!insertVariables(atom,variableToCheck,false))
			return false;
	for(auto atom:body)
		if(atom->isNegative()){
			if(!insertVariables(atom,variableToCheck,false))
				return false;
		}
		else
			insertVariables(atom,variableInPositiveAtom,true);
	if(variableInPositiveAtom.size()<variableToCheck.size())
		return false;
	for(auto term:variableToCheck)
		if(variableInPositiveAtom.count(term)!=1)
			return false;
	return true;
}

bool Rule::operator ==(const Rule& r) {
	  if(getSizeBody()!=r.getSizeBody())return false;
	  if(getSizeHead()!=r.getSizeHead())return false;
	  for(auto atom:head){
		  bool find=false;
		  for(auto it=r.getBeginHead();it!=r.getEndHead();it++)
			  if(*atom==*(*it)){
				  find=true;
				  break;
			  }
		  if(!find)return false;
	  }
	  for(auto atom:body){
		  bool find=false;
		  for(auto it=r.getBeginBody();it!=r.getEndBody();it++)
			  if(*atom==*(*it)){
				  find=true;
				  break;
			  }
		  if(!find)return false;
	  }

	  return true;

}

Rule::~Rule() {
	for(auto atom:head)delete atom;
	for(auto atom:body)delete atom;
}
