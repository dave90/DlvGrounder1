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

#include "../utility/IndexDefinition.h"

using namespace std;

enum AggregateFunction {NONE=0,COUNT,MAX,MIN,SUM};

enum Binop {NONE_OP=0,EQUAL,UNEQUAL,LESS,GREATER,LESS_OR_EQ,GREATER_OR_EQ};

class Atom {
public:
	Atom() {};
	Atom( bool negative){};
	Atom(index_object predicate);


	// Two atoms are equal if they have the same index
	virtual bool operator==(const Atom& a)=0;

	//Compute a hash of atom
	virtual size_t getHash() const = 0 ;

	//ClassicalLiteral
	virtual pair<bool,index_object> getPredicate() const {return {false,0};};
	virtual void setPredicate(index_object predicate) {};
	virtual vector<index_object>& getTerms() { void(0);};
	virtual const unsigned int getTermsSize() const {return 0;};
	virtual index_object getTerm(int i) const{return 0;};
	virtual void setTerms(const vector<index_object>& terms){};
	virtual bool isHasMinus() const {return 0;};
	virtual void setHasMinus(bool hasMinus) {};
	virtual bool isNegative() const {return 0;};
	virtual void setNegative(bool negative) {};

	//BuiltInAtom
	virtual Binop getBinop() const {return Binop::NONE_OP;};
	virtual void setBinop(Binop binop) {};
	virtual index_object getFirstTerm() const {return 0;};
	virtual void setFirstTerm(index_object firstTerm) {};
	virtual index_object getSecondTerm() const {return 0;};
	virtual void setSecondTerm(index_object secondTerm) {};

	//AggregateAtom
	virtual const vector<AggregateElement> getAggregateElements() const {return vector<AggregateElement>(0);};
	virtual void setAggregateElements(const vector<AggregateElement> aggregateElements) {};
	virtual AggregateFunction getAggregateFunction() const {return AggregateFunction::NONE;};
	virtual void setAggregateFunction(AggregateFunction aggregateFunction) {};
	virtual Binop getFirstBinop() const {return Binop::NONE_OP;};
	virtual void setFirstBinop(Binop firstBinop) {};
	virtual index_object getFirstCompareTerm() const {return 0;};
	virtual void setFirstCompareTerm(index_object firstCompareTerm) {};
	virtual Binop getSecondBinop() const {return Binop::NONE_OP;};
	virtual void setSecondBinop(Binop secondBinop) {};
	virtual index_object getSecondCompareTerm() const {return 0;};
	virtual void setSecondCompareTerm(index_object secondCompareTerm) {};

	//Choice
	virtual const vector<vector<index_object>> getChoiceElements() const {return vector<vector<index_object>>(0);};
	virtual void setChoiceElements(const vector<vector<index_object> > choiceElements) {};


	virtual void print();
	virtual void print(TermTable*tb){};
	virtual ~Atom(){};
};

#endif /* ATOM_H_ */
