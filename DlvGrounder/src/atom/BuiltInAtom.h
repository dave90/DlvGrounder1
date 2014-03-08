/*
 * BuiltInAtom.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef BUILTINATOM_H_
#define BUILTINATOM_H_

#include "NafLiteral.h"

class BuiltInAtom: public Atom {
public:
	BuiltInAtom(): firstTerm(0), secondTerm(0), negative(false) {};
	BuiltInAtom(unsigned long firstTerm, unsigned long secondTerm, string binop, bool negative): firstTerm(firstTerm), secondTerm(secondTerm), binop(binop), negative(negative)  {};

	const string& getBinop() const;
	void setBinop(const string& binop);
	unsigned long getFirstTerm() const;
	void setFirstTerm(unsigned long firstTerm);
	unsigned long getSecondTerm() const;
	void setSecondTerm(unsigned long secondTerm);
	bool isNegative() const;
	void setNegative(bool negative);

	virtual ~BuiltInAtom();

private:
	unsigned long firstTerm;
	unsigned long secondTerm;
	string binop;
	bool negative;
};

#endif /* BUILTINATOM_H_ */
