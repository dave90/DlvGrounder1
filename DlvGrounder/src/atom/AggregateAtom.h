/*
 * AggregateAtom.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef AGGREGATEATOM_H_
#define AGGREGATEATOM_H_

#include <vector>
#include <string>
#include "AggregateElement.h"
#include "Atom.h"
using namespace std;

class AggregateAtom: public Atom {
public:
	AggregateAtom(): firstCompareTerm(0), secondCompareTerm(0), negative(false) {};
	AggregateAtom(unsigned long f, unsigned long s, bool n): firstCompareTerm(f), secondCompareTerm(s), negative(n) {};

	const string& getAggregateFunction() const;
	void setAggregateFunction(const string& agggregateFunction);
	const vector<AggregateElement>& getAggregateElements() const;
	void setAggregateElements(
			const vector<AggregateElement>& aggregateElements);
	const string& getFirstBinop() const;
	void setFirstBinop(const string& firstBinop);
	unsigned long getFirstCompareTerm() const;
	void setFirstCompareTerm(unsigned long firstCompareTerm);
	const string& getSecondBinop() const;
	void setSecondBinop(const string& secondBinop);
	unsigned long getSecondCompareTerm() const;
	void setSecondCompareTerm(unsigned long secondCompareTerm);
	bool isNegative() const;
	void setNegative(bool negative);

	virtual ~AggregateAtom();

private:
	unsigned long firstCompareTerm;
	string firstBinop;
	unsigned long secondCompareTerm;
	string secondBinop;
	string aggregateFunction;
	vector<AggregateElement> aggregateElements;
	bool negative;
};

#endif /* AGGREGATEATOM_H_ */
