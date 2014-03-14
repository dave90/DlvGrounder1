/*
 * ClassicalLiteral.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "ClassicalLiteral.h"

const vector<unsigned long>& ClassicalLiteral::getTerms() const {
	return terms;
}

void ClassicalLiteral::setTerms(const vector<unsigned long>& terms) {
	this->terms = terms;
}

bool ClassicalLiteral::isHasMinus() const {
	return hasMinus;
}

void ClassicalLiteral::setHasMinus(bool hasMinus) {
	this->hasMinus = hasMinus;
}

unsigned long ClassicalLiteral::getPredicate() const {
	return predicate;
}

void ClassicalLiteral::setPredicate(unsigned long predicate) {
	this->predicate = predicate;
}

bool ClassicalLiteral::isNegative() const {
	return negative;
}

void ClassicalLiteral::setNegative(bool negative) {
	this->negative = negative;
}

ClassicalLiteral::~ClassicalLiteral(){}
