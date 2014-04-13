/*
 * PredicateTable.h
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#ifndef PREDICATETABLE_H_
#define PREDICATETABLE_H_

#include <unordered_set>

#include "../atom/Predicate.h"

/*
 * Hash function for the class Predicate
 */
struct hashPredicate {
	size_t operator()(Predicate* p) const {
		return p->getIndex();
	}
	bool operator()(Predicate *p1, Predicate *p2) const {
		return *p1 == *p2;
	}

};

using namespace std;

class PredicateTable {
public:
	PredicateTable();
	unsigned long insertPredicate(Predicate *p);
	Predicate* getPredicate(unsigned long id);
	void setEdb(unsigned long index);
	void setIdb(unsigned long index);
	~PredicateTable(){for(Predicate*p:hash)delete p;}
private:
	unordered_set<Predicate*, hashPredicate, hashPredicate> hash;
};

#endif /* PREDICATETABLE_H_ */
