/*
 * VariableTerm.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "VariableTerm.h"

VariableTerm::VariableTerm() {

}

string VariableTerm::getName() {
	return variable;
}

void VariableTerm::setName(string name) {
	variable=name;
}


void VariableTerm::addTerm(unsigned long termIndex) {
}

vector<unsigned long> VariableTerm::getTerms() {
	vector<unsigned long> v;
	return v;
}

void VariableTerm::print() {
	cout<<"Variable Term "<<variable<<" index: "<<getIndex()<<" Negation: "<<isNegative()<<endl;
}
