/*
 * FunctionTerm.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "FunctionTerm.h"

#include <sstream>
#include <boost/lexical_cast.hpp>



size_t FunctionTerm::hash() {
	vector<size_t> hashVec(terms.size()+1);
	hashVec[0]=HashString::getHashStringFromConfig()->computeHash(name);
	for(unsigned int i=0;i<terms.size();i++)
		hashVec[i+1]=terms[i]->getIndex();

	return HashVecInt::getHashVecIntFromConfig()->computeHashSize_T(hashVec);

}

void FunctionTerm::print() {
	cout<<name<<"(";
	for(unsigned int i=0;i<terms.size();i++){
		if(i!=0)
			cout<<",";
		terms[i]->print();
	}
	cout<<")";
}

Term* FunctionTerm::substitute(map_term_term& substritutionTerm) {
	// Create a new function replacing the term in a vector
	// Recursively call substitute for nested function
	// At the end add a new function in a table and return index

	TermTable *termTable=TermTable::getInstance();
	vector<Term*> subTerms(terms.size());
	for(unsigned int i=0;i<terms.size();i++){
		Term* sub_term=terms[i]->substitute(substritutionTerm);
		subTerms[i]=sub_term;
	}

	Term *newTerm=new FunctionTerm(name,terms);
	return TermTable::getInstance()->addTerm(newTerm);
}

Term* FunctionTerm::calculate() {
	// Create a new function replacing the term in a vector
	// Recursively call calculate for nested function
	// At the end add a new function in a table and return index
	if(!contain(TermType::ARITH)) return this;

	vector<Term*> sub_terms(terms.size());

	TermTable *termTable=TermTable::getInstance();

	for(unsigned int i=0;i<terms.size();i++){
		if(terms[i]->contain(TermType::ARITH)){
			sub_terms[i]=terms[i]->calculate();
		}else
			sub_terms[i]=terms[i];
	}

	Term *subTerm=new FunctionTerm(name,negative,sub_terms);
	return termTable->addTerm(subTerm);
}

bool FunctionTerm::match(Term* termToMatch, map_term_term& varAssignment) {
	// If is anonymus return true, if have same name and same arity continue and
	// recursivley check match, if one fail return true

	TermTable *termTable=TermTable::getInstance();

	if(termToMatch->getType()==TermType::ANONYMOUS) return true;
	if(termToMatch->getType()==TermType::VARIABLE){ varAssignment.insert({termToMatch,this});return true;}
	if(termToMatch->getType()!=TermType::FUNCTION) return false;
	if(termToMatch->getName().compare(getName()) != 0)return false;
	if(termToMatch->getTermsSize() != terms.size())return false;

	map_term_term assignInTerm(varAssignment);
	for(unsigned int i=0;i<terms.size();i++)
		if(!terms[i]->match(termToMatch->getTerm(i),assignInTerm))
			return false;

	varAssignment.insert(assignInTerm.begin(),assignInTerm.empty());

	return true;
}
