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

bool Rule::isSafe()
{
	vector<Term> VariableInNegativeAtom;
	vector<Term> VariableInPositiveAtom;
	if(isAStrongConstraint())				//primo caso testa vuota
	{
		for(auto atom:body)
		{
			if(atom->isNegative())
			{
				for(auto VariableTerm:atom->getVariable())
				{
					VariableInNegativeAtom.push_back(VariableTerm);
				}
			}
			else			//riempio i vector con le variabili
			{
				for(auto VariableTerm:atom->getVariable())
				{
					VariableInPositiveAtom.push_back((VariableTerm));
				}
			}
		}
		bool trovato=false;			//controllo se ogni variabile che compare in un atomo negativo
									// compare almeno una volta in un atomo positivo
		for(int i=0;i<VariableInNegativeAtom.size();i++)
		{
			for(int j=0;j<VariableInPositiveAtom.size();j++)
			{
				if(VariableInNegativeAtom[i].getName().compare("_")==0)
					return false;
				if(VariableInNegativeAtom[i].getName().compare(VariableInPositiveAtom[j].getName())==0)
				{
					trovato=true;
					break;
				}
			}
			if(!trovato)
				return false;
			trovato=false;
		}
		return true;
	}
	return false;
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
