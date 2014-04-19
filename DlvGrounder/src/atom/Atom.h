/*
 * Atom.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 *
 *      The hierarchy (at the moment) is this:
 *      									Atom
 *      	|					|				|					|
 *   	ClassicalLiteral	BuiltInAtom		AggregateAtom		Choice
 */

#ifndef ATOM_H_
#define ATOM_H_

#include <vector>
#include <string>
#include "AggregateElement.h"

#include "../table/TermTable.h"

using namespace std;

enum AggregateFunction {NONE=0,COUNT,MAX,MIN,SUM};

enum Binop {NONE_OP=0,EQUAL,UNEQUAL,LESS,GREATER,LESS_OR_EQ,GREATER_OR_EQ};

class Atom {
public:
	Atom() : index(0) {};
	Atom(bool negative): index(0) {};
	Atom(unsigned long index, bool negative): index(index) {};
	Atom(unsigned long predicate);

	//Getters and Setters
	unsigned long getIndex() const {return index;};
	void setIndex(unsigned long index) {this->index=index;};

	// Two atoms are equal if they have the same index
	bool operator==(const Atom& a);

	//Compute a string to be used for hashing
	virtual string getNameToHash() = 0 ;

	//ClassicalLiteral
	virtual unsigned long getPredicate() const {return -1;};
	virtual void setPredicate(unsigned long predicate) {};
	virtual const vector<unsigned long> getTerms() const {return vector<unsigned long>(0);};
	virtual void setTerms(const vector<unsigned long>& terms){};
	virtual bool isHasMinus() const {return 0;};
	virtual void setHasMinus(bool hasMinus) {};
	virtual bool isNegative() const {return 0;};
	virtual void setNegative(bool negative) {};

	//BuiltInAtom
	virtual Binop getBinop() const {return Binop::NONE_OP;};
	virtual void setBinop(Binop binop) {};
	virtual unsigned long getFirstTerm() const {return 0;};
	virtual void setFirstTerm(unsigned long firstTerm) {};
	virtual unsigned long getSecondTerm() const {return 0;};
	virtual void setSecondTerm(unsigned long secondTerm) {};

	//AggregateAtom
	virtual const vector<AggregateElement> getAggregateElements() const {return vector<AggregateElement>(0);};
	virtual void setAggregateElements(const vector<AggregateElement> aggregateElements) {};
	virtual AggregateFunction getAggregateFunction() const {return AggregateFunction::NONE;};
	virtual void setAggregateFunction(AggregateFunction aggregateFunction) {};
	virtual Binop getFirstBinop() const {return Binop::NONE_OP;};
	virtual void setFirstBinop(Binop firstBinop) {};
	virtual unsigned long getFirstCompareTerm() const {return 0;};
	virtual void setFirstCompareTerm(unsigned long firstCompareTerm) {};
	virtual Binop getSecondBinop() const {return Binop::NONE_OP;};
	virtual void setSecondBinop(Binop secondBinop) {};
	virtual unsigned long getSecondCompareTerm() const {return 0;};
	virtual void setSecondCompareTerm(unsigned long secondCompareTerm) {};

	//Choice
	virtual const vector<vector<unsigned long>> getChoiceElements() const {return vector<vector<unsigned long>>(0);};
	virtual void setChoiceElements(const vector<vector<unsigned long> > choiceElements) {};

	virtual void print();
	virtual void print(TermTable*tb){};
	virtual ~Atom(){};
private:
	unsigned long index;
};

#endif /* ATOM_H_ */
