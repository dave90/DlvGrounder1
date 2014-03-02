/*
 * FunctionTerm.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "FunctionTerm.h"

FunctionTerm::FunctionTerm() {

}


string FunctionTerm::getName() {
	return name;
}

void FunctionTerm::setName(string constant) {
	name=constant;
}

void FunctionTerm::addTerm(unsigned long termIndex) {
	terms.push_back(termIndex);
}

vector<unsigned long> FunctionTerm::getTerms() {
	return terms;
}

void FunctionTerm::print() {
	cout<<"Function Term "<<name<<" index: "<<getIndex()<<" Negation: "<<isNegative()<<" "<<" Terms:";
	for(int i=0;i<terms.size();i++)
		cout<<terms[i]<<" ";
	cout<<endl;
}
