/*
 * ConstantTerm.cpp
 *
 *  Created on: 27/feb/2014
 *      Author: Davide
 */


#include "ConstantTerm.h"


#include <stdlib.h>

ConstantTerm::ConstantTerm() {
	// TODO Auto-generated constructor stub

}


string ConstantTerm::getName() {
	return constant;
}

void ConstantTerm::setName(string constant) {
	this->constant=constant;
}

void ConstantTerm::addTerm(unsigned long termIndex) {
}

vector<unsigned long> ConstantTerm::getTerms() {
	vector<unsigned long> v;
	return v;
}

string ConstantTerm::getNameToHash() {
	return constant;
}

void ConstantTerm::print() {
	cout<<"Constant Term "<<constant<<" index: "<<getIndex()<<" Negation: "<<isNegative()<<endl;
}

double ConstantTerm::calculate() {
	return atof(constant.c_str());
}
