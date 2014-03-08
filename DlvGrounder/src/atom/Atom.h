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

using namespace std;

class Atom {
public:
	Atom() : index(0) {};
	Atom(bool negative): index(0) {};
	Atom(unsigned long index, bool negative): index(index) {};
	Atom(unsigned long predicate);

	//Getters and Setters
	unsigned long getIndex() const;
	void setIndex(unsigned long index);

	// Two atoms are equal if they have the same index
	bool operator==(const Atom& a);

	/*
	 * 	TODO Virtual methods for fields that are not common to the whole hierarchy should be put here?
	 * 	Like for predicate?
	*/
	virtual unsigned long getPredicate() const;
	virtual void setPredicate(unsigned long predicate);

	virtual void print();
	virtual ~Atom();
private:
	unsigned long index;
};

#endif /* ATOM_H_ */
