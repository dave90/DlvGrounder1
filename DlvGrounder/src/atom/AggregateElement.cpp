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

const vector<index_object>& AggregateElement::getNafLiterals() const {
	return nafLiterals;
}

void AggregateElement::setNafLiterals(
		const vector<index_object>& nafLiterals) {
	this->nafLiterals = nafLiterals;
}

const vector<index_object>& AggregateElement::getTerms() const {
	return terms;
}

void AggregateElement::setTerms(const vector<index_object>& terms) {
	this->terms = terms;
}
