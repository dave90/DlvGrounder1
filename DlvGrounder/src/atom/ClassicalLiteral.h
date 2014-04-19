/*
 * ClassicalLiteral.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#ifndef CLASSICALLITERAL_H_
#define CLASSICALLITERAL_H_

#include <vector>
#include "Atom.h"
using namespace std;

class ClassicalLiteral : public Atom {
public:
	ClassicalLiteral(): predicate(0), hasMinus(false), negative(false){};
	ClassicalLiteral(unsigned long p, vector<unsigned long> t, bool hM, bool n): predicate(p), terms(t), hasMinus(hM), negative(n) {};

	const vector<unsigned long> getTerms() const {return terms;};
	void setTerms(const vector<unsigned long>& terms){this->terms=terms;};
	unsigned long getPredicate() const {return predicate;};
	void setPredicate(unsigned long predicate) {this->predicate=predicate;};
	bool isHasMinus() const {return hasMinus;};
	void setHasMinus(bool hasMinus) {this->hasMinus=hasMinus;};
	bool isNegative() const {return negative;};
	void setNegative(bool negative) {this->negative=negative;};
	void print(TermTable *tb);

	string getNameToHash();


	//TODO metodi accesso lista termini
	~ClassicalLiteral() {};

private:
	unsigned long predicate;
	vector<unsigned long> terms;
	bool hasMinus;
	bool negative;
};

#endif /* CLASSICALLITERAL_H_ */
