/*
 * Term.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "Term.h"

Term::Term() {
	index=-1;
	negative=false;
}

Term::Term(string name) {
	setName(name);
}

Term::Term(string name, bool negatine) {
	setName(name);
	setNegative(negatine);
}


unsigned long Term::getIndex() {
	return index;
}

void Term::setIndex(unsigned long index) {
	this->index=index;
}

bool Term::operator ==(const Term& t) {
	return t.index==index;
}

bool Term::isNegative() {
	return negative;
}

void Term::setNegative(bool n) {
	negative=n;
}

void Term::setName(string name) {
}

void Term::addTerm(unsigned long termIndex) {
}

vector<unsigned long> Term::getTerms() {
}

void Term::print() {
}

string Term::getName() {
}

string Term::getNameToHash() {
}
