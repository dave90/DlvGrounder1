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
