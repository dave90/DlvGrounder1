/*
 * PredicateTable.h
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#ifndef PREDICATETABLE_H_
#define PREDICATETABLE_H_

#include <unordered_set>
#include <iostream>

#include "../atom/Predicate.h"

/**
 * Hash function for the class Predicate
 */
struct hashPredicate {
	size_t operator()(Predicate* p) const {
		return p->getIndex();
	}
	bool operator()(Predicate *p1, Predicate *p2) const {
		return p1->getIndex() == p2->getIndex();
	}

};

using namespace std;

/**
 * Hash table of predicate
 */
class PredicateTable {
public:
	PredicateTable(){};
	/// Insert predicate in a table and return the index of the predicate
	index_object insertPredicate(Predicate *p);
	/// Get the predicate with the specified index
	Predicate* getPredicate(index_object id);
	/// Set Edb the predicate with the specified index
	void setEdb(index_object index);
	/// Set Idb the predicate with the specified index
	void setIdb(index_object index);
	/// Get all the Edb predicate
	void getEdbPredicate(unordered_set<index_object>& edb_pred);
	~PredicateTable(){for(Predicate*p:hash)delete p;}
private:
	unordered_set<Predicate*, hashPredicate, hashPredicate> hash;
};

#endif /* PREDICATETABLE_H_ */
