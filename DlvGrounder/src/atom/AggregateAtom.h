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
	AggregateAtom(index_object f, Binop fB, index_object s, Binop sB, AggregateFunction aF, bool n):
		firstCompareTerm(f), firstBinop(fB), secondCompareTerm(s), secondBinop(sB), aggregateFunction(aF), negative(n) {};

	const vector<AggregateElement> getAggregateElements() const {return aggregateElements;};
	void setAggregateElements(const vector<AggregateElement>& aggregateElements) {this->aggregateElements = aggregateElements;};
	AggregateFunction getAggregateFunction() const {return aggregateFunction;};
	void setAggregateFunction(AggregateFunction aggregateFunction) {this->aggregateFunction = aggregateFunction;};
	Binop getFirstBinop() const {return firstBinop;};
	void setFirstBinop(Binop firstBinop) {this->firstBinop = firstBinop;};
	index_object getFirstCompareTerm() const {return firstCompareTerm;};
	void setFirstCompareTerm(index_object firstCompareTerm) {this->firstCompareTerm = firstCompareTerm;};
	bool isNegative() const {return negative;};
	void setNegative(bool negative) {this->negative = negative;};
	Binop getSecondBinop() const {return secondBinop;};
	void setSecondBinop(Binop secondBinop) {this->secondBinop = secondBinop;};
	index_object getSecondCompareTerm() const {return secondCompareTerm;};
	void setSecondCompareTerm(index_object secondCompareTerm) {this->secondCompareTerm = secondCompareTerm;};

	size_t getHash() const;

	~AggregateAtom() {};

private:
	index_object firstCompareTerm;
	Binop firstBinop;
	index_object secondCompareTerm;
	Binop secondBinop;
	AggregateFunction aggregateFunction;
	vector<AggregateElement> aggregateElements;
	bool negative;
};

#endif /* AGGREGATEATOM_H_ */
