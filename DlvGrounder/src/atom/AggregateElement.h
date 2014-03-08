/*
 * AggregateElement.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef AGGREGATEELEMENT_H_
#define AGGREGATEELEMENT_H_

#include <vector>
using namespace std;

class AggregateElement {
public:
	AggregateElement();

	const vector<unsigned long>& getNafLiterals() const;
	void setNafLiterals(const vector<unsigned long>& nafLiterals);
	const vector<unsigned long>& getTerms() const;
	void setTerms(const vector<unsigned long>& terms);

	virtual ~AggregateElement();
private:
	vector<unsigned long> terms;
	vector<unsigned long> nafLiterals;
};

#endif /* AGGREGATEELEMENT_H_ */
