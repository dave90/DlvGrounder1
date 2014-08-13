/*
 * FunctionTerm.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "FunctionTerm.h"

#include <sstream>
#include <boost/lexical_cast.hpp>



string FunctionTerm::getNameToHash() {
	string hashString=name+"*";

	string number;
	for(unsigned int i=0;i<terms.size();i++){
		hashString+=boost::lexical_cast<string>(terms[i])+"*";
	}

	return hashString;
}

void FunctionTerm::print() {
	cout<<IdsManager::getStringStrip(IdsManager::TERM_ID_MANAGER,getIndex())<<"(";
	for(unsigned int i=0;i<terms.size();i++){
		if(i!=0)
			cout<<",";
		TermTable::getInstance()->getTerm(terms[i])->print();
	}
	cout<<")";
}

index_object FunctionTerm::substitute(unordered_map<index_object, index_object>& substritutionTerm) {
	// Create a new function replacing the term in a vector
	// Recursively call substitute for nested function
	// At the end add a new function in a table and return index

	Term *subTerm=new FunctionTerm(name,negative);

	TermTable *termTable=TermTable::getInstance();
	for(auto term:terms){
		index_object sub_index=termTable->getTerm(term)->substitute(substritutionTerm);
		subTerm->addTerm(sub_index);
	}
	return termTable->addTerm(subTerm);
}

bool FunctionTerm::match(index_object termToMatch, unordered_map<index_object, index_object>& varAssignment) {
	// If is anonymus return true, if have same name and same arity continue and
	// recursivley check match, if one fail return true

	TermTable *termTable=TermTable::getInstance();

	Term * term=termTable->getTerm(termToMatch);

	if(term->isAnonymous()) return true;
	if(term->isVariableTerm()){ varAssignment.insert({termToMatch,getIndex()});return true;}
	if(!term->isFunctionalTerm()) return false;
	if(term->getName().compare(getName()) != 0)return false;
	if(term->getTerms().size() != terms.size())return false;

	unordered_map<index_object, index_object> assignInTerm(varAssignment);
	for(unsigned int i=0;i<terms.size();i++)
		if(!termTable->getTerm(terms[i])->match(term->getTerms()[i],assignInTerm))
			return false;

	for(auto assignment:assignInTerm)varAssignment.insert(assignment);

	return true;
}
