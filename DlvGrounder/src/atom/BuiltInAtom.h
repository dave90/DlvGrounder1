/*
 * BuiltInAtom.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef BUILTINATOM_H_
#define BUILTINATOM_H_

#include "Atom.h"
using namespace std;

/**
 * This class implements a built-in atom extending atom
 */

class BuiltInAtom: public Atom {
public:

	///Default constructor
	BuiltInAtom(): binop(Binop::NONE_OP), negative(false) {};

	/** Constructor
	 * @param binop set the binary operation @see Binop
	 * @param negative set whether the atom is negated with negation as failure
	 * @param firstTerm set the first term
	 * @param secondTerm set the second term
	 */
	BuiltInAtom(Binop binOperation, bool isNegative, Term* firstTerm, Term* secondTerm): binop(binOperation), negative(isNegative),terms(2) { terms[0]=firstTerm;terms[1]=secondTerm;};

	/** Constructor
	 * @param binop set the binary operation @see Binop
	 * @param negative set whether the atom is negated with negation as failure
	 * @param termsVec set the terms
	 */
	BuiltInAtom(Binop binOperation, bool isNegative, vector<Term*>& termsVec): Atom(termsVec), binop(binOperation), negative(isNegative) {};

	///Getter method for the binary operation
	Binop getBinop() const {return binop;};
	///Setter method for the binary operation
	void setBinop(Binop binop) {this->binop = binop;};
	///Returns true if the atom is negated with negation as failure
	bool isNegative() const {return negative;};
	///Set whether the atom is negated with negation as failure
	void setNegative(bool negative) {this->negative = negative;};

	///This method compute the resulting hash a built-in atom using its terms
	size_t hash();

	/** @brief Equal-to operator for built-in atom
	 * 	@details {Two built-in atoms are compared by the binary operation and the terms}
	 */
	virtual bool operator==(const Atom& a);

	///This method evaluate the truth value of the built-in atom
	bool evaluate(map_term_term& substitutionTerm);

	/// Return true if is BuiltInAtom
	virtual bool isBuiltIn(){return true;};

	///Printer Method
	void print();

	/// Substitute the term and return a new Atom with term substituted
	virtual Atom* substitute(map_term_term& substritutionTerm);

	///Destructor
	~BuiltInAtom() {};

private:
	///Binary operation @see Binop
	Binop binop;
	///Negated with true negation
	bool negative;
	//For the vector of terms, notice that the vector contains the terms in the same order as they appear: the first term in position 0, the second in position 1.
};

#endif /* BUILTINATOM_H_ */
