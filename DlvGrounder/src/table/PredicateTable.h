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


using namespace std;

/**
 * Hash table of predicate
 */
class PredicateTable {
public:
	PredicateTable(){};
	/// Insert predicate in a table and return the index of the predicate
	void insertPredicate(Predicate *&p){predicate_set.insert(p);};
	/// Get the predicate
	void getPredicate(Predicate*& p){predicate_set.get(p);};
	/// Set Edb the predicate with the specified index
	void setEdb(Predicate* p){predicate_set.get(p);p->setEdb();};
	/// Set Idb the predicate with the specified index
	void setIdb(Predicate* p){predicate_set.get(p);p->setIdb();};
	/// Get all the Edb predicate
	void getEdbPredicate(set_predicate& edb_pred);
	~PredicateTable(){}
private:
	FlyweightIndexFactory<Predicate> predicate_set;
};

#endif /* PREDICATETABLE_H_ */
