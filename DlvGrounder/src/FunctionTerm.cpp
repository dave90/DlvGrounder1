/*
 * FunctionTerm.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "FunctionTerm.h"

FunctionTerm::FunctionTerm() {

}

string FunctionTerm::getVariable() {
	return "";
}

void FunctionTerm::setVariable(string name) {
}

string FunctionTerm::getConstant() {
	return name;
}

void FunctionTerm::setCostant(string constant) {
	name=constant;
}

void FunctionTerm::addTerm(int termIndex) {
	terms.push_back(termIndex);
}

vector<int> FunctionTerm::getTerms() {
	return terms;
}

void FunctionTerm::print() {
	cout<<"Function Term "<<name<<" index: "<<getIndex()<<" "<<" Terms:";
	for(int i=0;i<terms.size();i++)
		cout<<terms[i]<<" ";
	cout<<endl;
}
