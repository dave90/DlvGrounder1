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

long Term::getIndex() {
	return index;
}

void Term::setIndex(long index) {
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
