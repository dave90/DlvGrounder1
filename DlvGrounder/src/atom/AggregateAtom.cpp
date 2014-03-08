/*
 * AggregateAtom.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "AggregateAtom.h"

const string& AggregateAtom::getAggregateFunction() const {
	return aggregateFunction;
}

void AggregateAtom::setAggregateFunction(const string& agggregateFunction) {
	this->aggregateFunction = agggregateFunction;
}

const vector<AggregateElement>& AggregateAtom::getAggregateElements() const {
	return aggregateElements;
}

void AggregateAtom::setAggregateElements(
		const vector<AggregateElement>& aggregateElements) {
	this->aggregateElements = aggregateElements;
}

const string& AggregateAtom::getFirstBinop() const {
	return firstBinop;
}

void AggregateAtom::setFirstBinop(const string& firstBinop) {
	this->firstBinop = firstBinop;
}

unsigned long AggregateAtom::getFirstCompareTerm() const {
	return firstCompareTerm;
}

void AggregateAtom::setFirstCompareTerm(unsigned long firstCompareTerm) {
	this->firstCompareTerm = firstCompareTerm;
}

const string& AggregateAtom::getSecondBinop() const {
	return secondBinop;
}

void AggregateAtom::setSecondBinop(const string& secondBinop) {
	this->secondBinop = secondBinop;
}

unsigned long AggregateAtom::getSecondCompareTerm() const {
	return secondCompareTerm;
}

void AggregateAtom::setSecondCompareTerm(unsigned long secondCompareTerm) {
	this->secondCompareTerm = secondCompareTerm;
}

bool AggregateAtom::isNegative() const {
	return negative;
}

void AggregateAtom::setNegative(bool negative) {
	this->negative = negative;
}
