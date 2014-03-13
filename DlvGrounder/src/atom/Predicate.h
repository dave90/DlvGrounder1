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
using namespace std;

class Predicate {
public:
	Predicate() : index(0), arity(0) {};
	Predicate(unsigned long index, string name, int arity) : index(index), arity(arity), name(name) {};

	//Getters and Setters
	int getArity();
	void setArity(int arity);
	string getName();
	void setName(string name);
	unsigned long getIndex() const;
	void setIndex(unsigned long index);
	bool operator==(Predicate &p);

	// Return the hash string name+arity
	string getNameToHash();

private:
	unsigned long index;
	int arity;
	string name;
};

#endif /* PREDICATE_H_ */