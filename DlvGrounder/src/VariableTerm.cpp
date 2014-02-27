/*
 * VariableTerm.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "VariableTerm.h"

VariableTerm::VariableTerm() {

}

string VariableTerm::getVariable() {
	return variable;
}

void VariableTerm::setVariable(string name) {
	variable=name;
}

string VariableTerm::getConstant() {
	return "";
}

void VariableTerm::setCostant(string constant) {
}

void VariableTerm::addTerm(int termIndex) {
}

vector<int> VariableTerm::getTerms() {
	vector<int> v;
	return v;
}

void VariableTerm::print() {
	cout<<"Variable Term "<<variable<<" index: "<<getIndex()<<endl;
}
