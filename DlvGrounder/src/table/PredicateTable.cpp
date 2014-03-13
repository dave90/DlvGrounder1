/*
 * PredicateTable.cpp
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#include "PredicateTable.h"

PredicateTable::PredicateTable() {
	// TODO Auto-generated constructor stub

}

unsigned long PredicateTable::insertPredicate(Predicate& p) {
	pair<unsigned long, bool> resultIdManager=manager.insert(p.getNameToHash());
	unsigned long index=resultIdManager.first;
	if(!resultIdManager.second){
		p.setIndex(index);
		hash.insert(p);
	}

	return index;


}

Predicate PredicateTable::getPredicate(unsigned long id) {

	Predicate p;
	p.setIndex(id);

	return *(hash.find(p));
}
