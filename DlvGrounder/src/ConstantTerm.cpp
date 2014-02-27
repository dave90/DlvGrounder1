/*
 * ConstantTerm.cpp
 *
 *  Created on: 27/feb/2014
 *      Author: Davide
 */

#include "ConstantTerm.h"

ConstantTerm::ConstantTerm() {
	// TODO Auto-generated constructor stub

}

string ConstantTerm::getVariable() {
	return "";
}

void ConstantTerm::setVariable(string name) {
}

string ConstantTerm::getConstant() {
	return constant;
}

void ConstantTerm::setCostant(string constant) {
	this->constant=constant;
}

void ConstantTerm::addTerm(int termIndex) {
}

vector<int> ConstantTerm::getTerms() {
	vector<int> v;
	return v;
}

void ConstantTerm::print() {
	cout<<"Constant Term "<<constant<<" index: "<<getIndex()<<endl;
}
