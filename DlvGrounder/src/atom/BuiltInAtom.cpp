/*
 * BuiltInAtom.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "BuiltInAtom.h"

const string& BuiltInAtom::getBinop() const {
	return binop;
}

void BuiltInAtom::setBinop(const string& binop) {
	this->binop = binop;
}

unsigned long BuiltInAtom::getFirstTerm() const {
	return firstTerm;
}

void BuiltInAtom::setFirstTerm(unsigned long firstTerm) {
	this->firstTerm = firstTerm;
}

unsigned long BuiltInAtom::getSecondTerm() const {
	return secondTerm;
}

void BuiltInAtom::setSecondTerm(unsigned long secondTerm) {
	this->secondTerm = secondTerm;
}

bool BuiltInAtom::isNegative() const {
	return negative;
}

void BuiltInAtom::setNegative(bool negative) {
	this->negative = negative;
}
