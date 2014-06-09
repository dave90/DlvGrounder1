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

/*
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

class PredicateTable {
public:
	PredicateTable();
	index_object insertPredicate(Predicate *p);
	Predicate* getPredicate(index_object id);
	void setEdb(index_object index);
	void setIdb(index_object index);
	void getEdbPredicate(unordered_set<index_object>& edb_pred);
	~PredicateTable(){for(Predicate*p:hash)delete p;}
private:
	unordered_set<Predicate*, hashPredicate, hashPredicate> hash;
};

#endif /* PREDICATETABLE_H_ */
