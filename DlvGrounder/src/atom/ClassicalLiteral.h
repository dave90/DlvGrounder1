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

	/** Constructor
	 * @param predicate set the predicate index
	 * @param terms set the terms vector
	 * @param hasMinus set whether the atom is negated with true negation
	 * @param negative set whether the atom is negated with negation as failure
	 */
	ClassicalLiteral(Predicate* predicate, vector<Term*> &terms, bool hasMinus, bool negative): Atom(terms), predicate(predicate), hasMinus(hasMinus), negative(negative) {};

	/** Getter method for the predicate
	 */
	Predicate* getPredicate() const {return predicate;};
	///Setter method for the predicate
	void setPredicate(Predicate* predicate) {this->predicate=predicate;};
	///Returns true if the atom is negated with true negation
	bool isHasMinus() const {return hasMinus;};
	///Set whether the atom is negated with true negation
	void setHasMinus(bool hasMinus) {this->hasMinus=hasMinus;};
	///Returns true if the atom is negated with negation as failure
	bool isNegative() const {return negative;};
	///Set whether the atom is negated with negation as failure
	void setNegative(bool negative) {this->negative=negative;};

	///This method compute the resulting hash a classical atom using its terms
	size_t hash() ;

	/** @brief Equal-to operator for classical atom
	 * 	@details{Two classical atoms are compared by the predicate and the terms.
	 * 	It is needed in order to compare facts. }
	 */
	bool operator==(const Atom& a);

	///This method is used in the printer method @see print()
	//It is static since it is used in Instance class, where just terms are stored instead of the whole classical atom
	static void print(Predicate* predicate,const vector<Term*>& terms,bool negative,bool hasMinus);
	///Printer Method
	void print();

	/// Substitute the term and return a new Atom with term substituted
	virtual Atom* substitute(map_term_term& substritutionTerm);



	///Destructor
	virtual ~ClassicalLiteral() {};

private:
	///Index of the predicate
	Predicate* predicate;
	///Negated with true negation
	bool hasMinus;
	///Negated with naf
	bool negative;
};

#endif /* CLASSICALLITERAL_H_ */
