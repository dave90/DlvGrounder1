/*
 * Predicate.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#ifndef PREDICATE_H_
#define PREDICATE_H_

#include "../utility/IndexDefinition.h"
#include <string>
using namespace std;

class Predicate {
public:

	///Default constructor
	Predicate() : index(0), arity(0), edb(true) {};

	/** Constructor
	 * @param name set the name of the predicate
	 * @param arity set the arity of the predicate
	 */
	Predicate(string name, unsigned int arity) : index(0), arity(arity), name(name), edb(true) {};

	/** Constructor
	 * @param name set the name of the predicate
	 * @param arity set the arity of the predicate
	 * @param edbIdb set whether the the predicate is an EDB or not
	 */
	Predicate(string name, unsigned int arity, bool edbIdb) : index(0), arity(arity), name(name), edb(edbIdb) {};

	///Getter method for the arity
	unsigned int getArity() const {return arity;}
	///Setter method for the arity
	void setArity(unsigned int arity) {this->arity = arity;}
	///Getter method for the arity
	bool isEdb() const {return edb;}

	void setIdb() {this->edb=false;};

	void setEdb() {this->edb=true;};
	///Getter method for the index
	index_object getIndex() const {return index;}
	///Setter method for the index
	void setIndex(index_object index) {this->index = index;}
	///Getter method for the name
	const string& getName() const {return name;}
	///Setter method for the name
	void setName(const string& name) {this->name = name;}

	/// @brief Equal-to operator for predicates
	/// @details Two predicates are equal if they have the same name and the same arity
	bool operator==(Predicate &p);

	///This method compute an hash for the predicate using its name and its arity
	string getNameToHash();

private:
	///Index of the predicate (assigned by the predicates' IdManager)
	index_object index;
	///Arity
	unsigned int arity;
	///Name
	string name;
	///Whether it is EDB or IDB
	bool edb;
};

#endif /* PREDICATE_H_ */
