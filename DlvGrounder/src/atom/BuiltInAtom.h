/*
 * BuiltInAtom.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef BUILTINATOM_H_
#define BUILTINATOM_H_

#include "Atom.h"
#include "../table/TermTable.h"
using namespace std;

class BuiltInAtom: public Atom {
public:
	BuiltInAtom():  firstTerm(0), secondTerm(0), binop(Binop::NONE_OP), negative(false) {};
	BuiltInAtom( index_object firstTerm, index_object secondTerm, Binop binop, bool negative):
		 firstTerm(firstTerm), secondTerm(secondTerm), binop(binop), negative(negative)  {};

	Binop getBinop() const {return binop;};
	void setBinop(Binop binop) {this->binop = binop;};
	index_object getFirstTerm() const {return firstTerm;};
	void setFirstTerm(index_object firstTerm) {this->firstTerm = firstTerm;};
	bool isNegative() const {return negative;};
	void setNegative(bool negative) {this->negative = negative;};
	index_object getSecondTerm() const {return secondTerm;};
	void setSecondTerm(index_object secondTerm) {this->secondTerm = secondTerm;};

	size_t getHash() const;

	virtual bool operator==(const Atom& a);


	bool evaluate();

	void print();
	~BuiltInAtom() {};

private:
	index_object firstTerm;
	index_object secondTerm;
	Binop binop;
	bool negative;
};

#endif /* BUILTINATOM_H_ */
