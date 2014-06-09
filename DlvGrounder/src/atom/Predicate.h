/*
 * Predicate.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#ifndef PREDICATE_H_
#define PREDICATE_H_

#include <string>
#include <cstring>

#include "../utility/IndexDefinition.h"


using namespace std;

class Predicate {
public:
	Predicate() : index(0), arity(0) {edb=true;};
	Predicate(string name, int arity) : index(0), arity(arity), name(name) {edb=true;};

	//Getters and Setters
	int getArity();
	void setArity(int arity);
	string getName();
	void setName(string name);
	index_object getIndex() const;
	void setIndex(index_object index);

	bool isEdb(){return edb;};
	void setIdb(){edb=false;};
	void setEdb(){edb=true;};


	bool operator==(Predicate &p);

	// Return the hash string name+arity
	string getNameToHash();

private:
	index_object index;
	int arity;
	string name;

	bool edb;
};

#endif /* PREDICATE_H_ */
