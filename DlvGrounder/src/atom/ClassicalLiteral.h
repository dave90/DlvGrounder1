/*
 * ClassicalLiteral.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#ifndef CLASSICALLITERAL_H_
#define CLASSICALLITERAL_H_

#include "Atom.h"
#include "../table/TermTable.h"

using namespace std;

/**
 * This class implements a classical atom extending atom
 */

class ClassicalLiteral : public Atom {
public:

	///Default constructor
	ClassicalLiteral(): predicate(0), hasMinus(false), negative(false){};

	/** Constructor
	 * @param predicate set the predicate index
	 * @param terms set the terms vector
	 * @param hasMinus set whether the atom is negated with true negation
	 * @param negative set whether the atom is negated with negation as failure
	 */
	ClassicalLiteral(index_object predicate, vector<index_object> &terms, bool hasMinus, bool negative): Atom(terms), predicate(predicate), hasMinus(hasMinus), negative(negative) {};

	/** Getter method for the predicate
	 * @retval <true,id> if the atom has a predicate whose index is id
	 * @retval <false,0> if the atom has not a predicate
	 */
	pair<bool,index_object> getPredicate() const {return {true,predicate};};
	///Setter method for the predicate
	void setPredicate(index_object predicate) {this->predicate=predicate;};
	///Returns true if the atom is negated with true negation
	bool isHasMinus() const {return hasMinus;};
	///Set whether the atom is negated with true negation
	void setHasMinus(bool hasMinus) {this->hasMinus=hasMinus;};
	///Returns true if the atom is negated with negation as failure
	bool isNegative() const {return negative;};
	///Set whether the atom is negated with negation as failure
	void setNegative(bool negative) {this->negative=negative;};

	///This method compute the resulting hash a classical atom using its terms
	size_t getHash() const;

	/** @brief Equal-to operator for classical atom
	 * 	@details{Two classical atoms are compared by the predicate and the terms.
	 * 	It is needed in order to compare facts. }
	 */
	bool operator==(const Atom& a);

	///This method is used in the printer method @see print()
	//It is static since it is used in Instance class, where just terms are stored instead of the whole classical atom
	static void print(index_object predicate,vector<index_object>& terms,bool negative,bool hasMinus);
	///Printer Method
	void print();

	/// Substitute the term and return a new Atom with term substituted
	virtual Atom* substitute(unordered_map<index_object, index_object>& substritutionTerm);



	///Destructor
	~ClassicalLiteral() {};

private:
	///Index of the predicate
	index_object predicate;
	///Negated with true negation
	bool hasMinus;
	///Negated with naf
	bool negative;
};

#endif /* CLASSICALLITERAL_H_ */
