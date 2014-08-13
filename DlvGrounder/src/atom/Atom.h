/*
 * Atom.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 *
 */

#ifndef ATOM_H_
#define ATOM_H_

class AggregateElement;
class ChoiceElement;

#include <vector>
#include <string>
#include <unordered_map>
#include "AggregateElement.h"
#include "ChoiceElement.h"
#include "../utility/IndexDefinition.h"

using namespace std;

/// Aggregate Function Types
enum AggregateFunction {NONE=0,COUNT,MAX,MIN,SUM};

/// Binary Operation Types
enum Binop {NONE_OP=0,EQUAL,UNEQUAL,LESS,GREATER,LESS_OR_EQ,GREATER_OR_EQ};

/**      The hierarchy is this:
*      									Atom
*      	|					|				|					|
*   	ClassicalLiteral	BuiltInAtom		AggregateAtom		Choice
*
*	Atom acts like an interface for its subclasses.
*/

class Atom {
public:

	///Default constructor
	Atom() {};

	/** Constructor
	 * @param termsVec set the terms vector
	 */
	Atom(vector<index_object> termsVec) : terms(move(termsVec)) {};

	/** Constructor
	 * @param negative set whether the atom is negated with negation as failure
	 */
	Atom(bool negative) {this->setNegative(negative);};

	/* Constructor
	 * @param predicate set the predicate index if any
	 */
	Atom(index_object predicate) {this->setPredicate(predicate);};

	/** Constructor
	 * @param negative set whether the atom is negated with negation as failure
	 * @param predicate set the predicate index if any
	 */
	Atom(bool negative, index_object predicate) {this->setNegative(negative); this->setPredicate(predicate);};

	///Equal-to operator for atoms
	virtual bool operator==(const Atom& a)=0;

	///This method compute the resulting hash for an atom using its fields
	virtual size_t getHash() const = 0 ;

	///Return the ID of variable present in the Atom
	virtual unordered_set<index_object> getVariable();

	/// Return true if is ground, each term is constant term
	virtual bool isGround(){
		for(index_object term:terms)
			if(TermTable::getInstance()->getTerm(term)->isConstant())return false;
		return true;
	}

	/******** Methods useful for ClassicalLiteral ********/
	/** Getter method for the predicate
	 * @retval <true,id> if the atom has a predicate whose index is id
	 * @retval <false,0> if the atom has not a predicate
	 */
	virtual pair<bool,index_object> getPredicate() const {return {false,0};};
	///Setter method for the predicate
	virtual void setPredicate(index_object predicate) {};
	///Getter method for the terms
	const vector<index_object>& getTerms() const {return terms;};
	///This method returns the size of the terms' vector
	const unsigned int getTermsSize() const {return terms.size();};
	/** This method returns the term in the position i
	 * @retval <true,t> if the position i is valid, and t is the term in that position
	 * @retval <false,0> if the position i is not valid
	 */
	pair<bool,index_object> getTerm(unsigned int i) const {if(i<terms.size()) return {true,terms[i]}; return {false,0};};
	///Setter method for the terms
	void setTerms(const vector<index_object>& terms) {this->terms = terms;};
	///Returns true if the atom is negated with true negation
	virtual bool isHasMinus() const {return 0;};
	///Set whether the atom is negated with true negation
	virtual void setHasMinus(bool hasMinus) {};
	///Returns true if the atom is negated with negation as failure
	virtual bool isNegative() const {return 0;};
	///Set whether the atom is negated with negation as failure
	virtual void setNegative(bool negative) {};
	/*****************************************************/

	/******** Methods useful for BuiltInAtom ********/
	///Getter method for the binary operation
	virtual Binop getBinop() const {return Binop::NONE_OP;};
	///Setter method for the binary operation
	virtual void setBinop(Binop binop) {};
	/// Return true if is BuiltInAtom
	virtual bool isBuiltIn(){return false;};
	///This method evaluate the truth value of the built-in atom
	virtual bool evaluate(){return false;};
	/*****************************************************/

	/******** Methods useful for AggregateAtom ********/
	///Getter method for the aggregate elements
	virtual vector<AggregateElement> getAggregateElements() {return vector<AggregateElement>(0);};
	///Setter method for the aggregate elements
	virtual void setAggregateElements(const vector<AggregateElement>& aggregateElements) {};
	///Getter method for the aggregate function
	virtual AggregateFunction getAggregateFunction() const {return AggregateFunction::NONE;};
	///Setter method for the aggregate function
	virtual void setAggregateFunction(AggregateFunction aggregateFunction) {};
	///Getter method for the first binary operation
	virtual Binop getFirstBinop() const {return Binop::NONE_OP;};
	///Setter method for the first binary operation
	virtual void setFirstBinop(Binop firstBinop) {};
	///Getter method for the second binary operation
	virtual Binop getSecondBinop() const {return Binop::NONE_OP;};
	///Setter method for the second binary operation
	virtual void setSecondBinop(Binop secondBinop) {};
	/*****************************************************/

	/******** Methods useful for Choice ********/
	///Getter method for the choice elements
	virtual const vector<ChoiceElement> getChoiceElements() const {return vector<ChoiceElement>(0);};
	///Setter method for the choice elements
	virtual void setChoiceElements(const vector<vector<index_object> >& choiceElements) {};
	/*****************************************************/

	/// Substitute the terms in the atom with the given terms and return the atom with terms sobstitute
	/// @param substritutionTerm map of index_object. The first index is the ID of term to substitute and second the value
	virtual Atom* substitute(unordered_map<index_object, index_object>& substritutionTerm){return nullptr;};
	/// Substitute the term with constant term and calculate the arithmetic terms
	/// The subclasses have to implement the substitute method for create correct type class of Atom
	virtual Atom* ground(unordered_map<index_object, index_object>& substritutionTerm);
	///Printer method
	virtual void print() = 0;
	///Destructor
	virtual ~Atom() {};

protected:
	vector<index_object> terms;
};

#endif /* ATOM_H_ */

