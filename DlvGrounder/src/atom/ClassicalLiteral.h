/*
 * ClassicalLiteral.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#ifndef CLASSICALLITERAL_H_
#define CLASSICALLITERAL_H_

#include <vector>
using namespace std;

class ClassicalLiteral : public Atom {
public:
	ClassicalLiteral(): predicate(0), hasMinus(false), negative(false){};
	ClassicalLiteral(unsigned long p, vector<unsigned long> t, bool hM, bool n): predicate(p), terms(t), hasMinus(hM), negative(n) {};

	const vector<unsigned long>& getTerms() const;
	void setTerms(const vector<unsigned long>& terms);
	unsigned long getPredicate() const;
	void setPredicate(unsigned long predicate);
	bool isHasMinus() const;
	void setHasMinus(bool hasMinus);
	bool isNegative() const;
	void setNegative(bool negative);

	//TODO metodi accesso lista termnini + commentare
	virtual ~ClassicalLiteral();

private:
	unsigned long predicate;
	vector<unsigned long> terms;
	bool hasMinus;
	bool negative;
};

#endif /* CLASSICALLITERAL_H_ */
