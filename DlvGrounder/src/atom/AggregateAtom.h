/*
 * AggregateAtom.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef AGGREGATEATOM_H_
#define AGGREGATEATOM_H_

#include <vector>
#include "Atom.h"
using namespace std;

class AggregateAtom: public Atom {
public:
	AggregateAtom(): firstCompareTerm(0), firstBinop(Binop::NONE_OP), secondCompareTerm(0), secondBinop(Binop::NONE_OP), aggregateFunction(AggregateFunction::NONE), negative(false) {};
	AggregateAtom(unsigned long f, Binop fB, unsigned long s, Binop sB, AggregateFunction aF, bool n):
		firstCompareTerm(f), firstBinop(fB), secondCompareTerm(s), secondBinop(sB), aggregateFunction(aF), negative(n) {};

	const vector<AggregateElement> getAggregateElements() const {return aggregateElements;};
	void setAggregateElements(const vector<AggregateElement>& aggregateElements) {this->aggregateElements = aggregateElements;};
	AggregateFunction getAggregateFunction() const {return aggregateFunction;};
	void setAggregateFunction(AggregateFunction aggregateFunction) {this->aggregateFunction = aggregateFunction;};
	Binop getFirstBinop() const {return firstBinop;};
	void setFirstBinop(Binop firstBinop) {this->firstBinop = firstBinop;};
	unsigned long getFirstCompareTerm() const {return firstCompareTerm;};
	void setFirstCompareTerm(unsigned long firstCompareTerm) {this->firstCompareTerm = firstCompareTerm;};
	bool isNegative() const {return negative;};
	void setNegative(bool negative) {this->negative = negative;};
	Binop getSecondBinop() const {return secondBinop;};
	void setSecondBinop(Binop secondBinop) {this->secondBinop = secondBinop;};
	unsigned long getSecondCompareTerm() const {return secondCompareTerm;};
	void setSecondCompareTerm(unsigned long secondCompareTerm) {this->secondCompareTerm = secondCompareTerm;};

	string getNameToHash();

	~AggregateAtom() {};

private:
	unsigned long firstCompareTerm;
	Binop firstBinop;
	unsigned long secondCompareTerm;
	Binop secondBinop;
	AggregateFunction aggregateFunction;
	vector<AggregateElement> aggregateElements;
	bool negative;
};

#endif /* AGGREGATEATOM_H_ */
