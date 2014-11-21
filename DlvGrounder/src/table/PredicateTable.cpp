/*
 * PredicateTable.cpp
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#include "PredicateTable.h"
#include "IdsManager.h"



void PredicateTable::getEdbPredicate(set_predicate& edb_pred) {
	for (auto p : predicate_set.flyweight_set)
		if (p->isEdb())
			edb_pred.insert(p);
}
