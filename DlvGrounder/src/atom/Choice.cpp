/*
 * Choice.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "Choice.h"

const string& Choice::getFirstBinop() const {
	return firstBinop;
}

void Choice::setFirstBinop(const string& firstBinop) {
	this->firstBinop = firstBinop;
}

unsigned long Choice::getFirstCompareTerm() const {
	return firstCompareTerm;
}

void Choice::setFirstCompareTerm(unsigned long firstCompareTerm) {
	this->firstCompareTerm = firstCompareTerm;
}

const string& Choice::getSecondBinop() const {
	return secondBinop;
}

void Choice::setSecondBinop(const string& secondBinop) {
	this->secondBinop = secondBinop;
}

unsigned long Choice::getSecondCompareTerm() const {
	return secondCompareTerm;
}

void Choice::setSecondCompareTerm(unsigned long secondCompareTerm) {
	this->secondCompareTerm = secondCompareTerm;
}

const vector<vector<unsigned long> >& Choice::getChoiceElements() const {
	return choiceElements;
}

void Choice::setChoiceElements(
		const vector<vector<unsigned long> >& choiceElements) {
	this->choiceElements = choiceElements;
}
