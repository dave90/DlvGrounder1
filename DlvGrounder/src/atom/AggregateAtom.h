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

/**
 * This class implements an aggregate atom extending atom
 */

class AggregateAtom: public Atom {
public:

	///Default constructor
	AggregateAtom(): firstBinop(Binop::NONE_OP), secondBinop(Binop::NONE_OP), aggregateFunction(AggregateFunction::NONE), negative(false) {};

	/** Constructor
		 * @param f set the first term of comparison
		 * @param fB set the first binary operation
		 * @param s set the second term of comparison
		 * @param sB set the second binary operation
		 * @param aF set the aggregate function type
		 * @param aE set the vector of aggregate elements
		 * @param negative set whether the atom is negated with negation as failure
		 */
	AggregateAtom(Term* f, Binop fB,Term* s, Binop sB, AggregateFunction aF, vector<AggregateElement> aE, bool n):
		firstBinop(fB), secondBinop(sB), aggregateFunction(aF), aggregateElements(move(aE)), negative(n) {terms.push_back(f);terms.push_back(s);};

	///Getter method for the aggregate elements
	vector<AggregateElement> getAggregateElements() {return aggregateElements;};
	///Setter method for the aggregate elements
	void setAggregateElements(const vector<AggregateElement>& aggregateElements) {this->aggregateElements = aggregateElements;};
	///Getter method for the aggregate function
	AggregateFunction getAggregateFunction() const {return aggregateFunction;};
	///Setter method for the aggregate function
	void setAggregateFunction(AggregateFunction aggregateFunction) {this->aggregateFunction = aggregateFunction;};
	///Getter method for the first binary operation
	Binop getFirstBinop() const {return firstBinop;};
	///Setter method for the first binary operation
	void setFirstBinop(Binop firstBinop) {this->firstBinop = firstBinop;};
	///Getter method for the second binary operation
	Binop getSecondBinop() const {return secondBinop;};
	///Setter method for the second binary operation
	void setSecondBinop(Binop secondBinop) {this->secondBinop = secondBinop;};
	///Returns true if the atom is negated with negation as failure
	bool isNegative() const {return negative;};
	///Set whether the atom is negated with negation as failure
	void setNegative(bool negative) {this->negative = negative;};

	///This method compute the resulting hash for an aggregateElement TODO
	size_t getHash() const {return 0;};

	///Printer method
	void print();

	~AggregateAtom() {	};

private:
	///First binary operation
	Binop firstBinop;
	///Second binary operation
	Binop secondBinop;
	///Aggregate function @see AggregateFunction
	AggregateFunction aggregateFunction;
	///Vector of the aggregate elements
	vector<AggregateElement> aggregateElements;
	///Negated with naf
	bool negative;

	/* For the vector of terms, it contains the first and the second term of comparison.
	 * Notice that the vector contains the terms in the same order as they appear: the first term in position 0, the second in position 1.
	 */
};

#endif /* AGGREGATEATOM_H_ */
