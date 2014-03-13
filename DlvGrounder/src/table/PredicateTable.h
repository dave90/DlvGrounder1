/*
 * PredicateTable.h
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#ifndef PREDICATETABLE_H_
#define PREDICATETABLE_H_

#include "../atom/Predicate.h"
#include "IdManager.h"

/*
 * Hash function for the class Predicate
 */
struct hashPredicate {
  size_t operator()(Predicate p) const {
	  return p.getIndex();
  }

};

/*
 * Equal function for the class Predicate
 */
struct equalPredicate{
	  bool operator()( Predicate p1,  Predicate p2)const{
		  return p1==p2;
	  }
};

using namespace std;

class PredicateTable {
public:
	PredicateTable();
	unsigned long insertPredicate(Predicate &p);
	Predicate getPredicate(unsigned long id);

private:
	IdManager manager;
	unordered_set<Predicate,hashPredicate,equalPredicate> hash;
};

#endif /* PREDICATETABLE_H_ */
