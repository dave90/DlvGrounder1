/*
 * AggregateElement.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef AGGREGATEELEMENT_H_
#define AGGREGATEELEMENT_H_

#include <vector>

#include "../utility/IndexDefinition.h"


using namespace std;

class AggregateElement {
public:
	AggregateElement();

	const vector<index_object>& getNafLiterals() const;
	void setNafLiterals(const vector<index_object>& nafLiterals);
	const vector<index_object>& getTerms() const;
	void setTerms(const vector<index_object>& terms);

	virtual ~AggregateElement();
private:
	vector<index_object> terms;
	vector<index_object> nafLiterals;
};

#endif /* AGGREGATEELEMENT_H_ */
