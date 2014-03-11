/*
 * AggregateElement.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "AggregateElement.h"

AggregateElement::AggregateElement(){

}

AggregateElement::~AggregateElement(){

}

const vector<unsigned long>& AggregateElement::getNafLiterals() const {
	return nafLiterals;
}

void AggregateElement::setNafLiterals(
		const vector<unsigned long>& nafLiterals) {
	this->nafLiterals = nafLiterals;
}

const vector<unsigned long>& AggregateElement::getTerms() const {
	return terms;
}

void AggregateElement::setTerms(const vector<unsigned long>& terms) {
	this->terms = terms;
}
