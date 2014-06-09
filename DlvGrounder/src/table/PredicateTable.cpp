/*
 * PredicateTable.cpp
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#include "PredicateTable.h"
#include "IdsManager.h"

PredicateTable::PredicateTable() {
	// TODO Auto-generated constructor stub

}

index_object PredicateTable::insertPredicate(Predicate* p) {
	string nameHash=p->getNameToHash();
	pair<index_object, bool> resultIdManager = IdsManager::getIndex(IdsManager::PREDICATE_ID_MANAGER, nameHash);
	index_object index = resultIdManager.first;
	if (!resultIdManager.second) {
		p->setIndex(index);
		hash.insert(p);
	} else
		delete p;

	return index;

}

Predicate* PredicateTable::getPredicate(index_object id) {

	Predicate p;
	p.setIndex(id);

	Predicate *p_finded = *hash.find(&p);
	return p_finded;
}

void PredicateTable::setEdb(index_object index) {
	Predicate p;
	p.setIndex(index);
	Predicate *tmp = *hash.find(&p);
	tmp->setEdb();
}

void PredicateTable::setIdb(index_object index) {
	Predicate *p = new Predicate;
	p->setIndex(index);
//	Predicate *tmp=*hash.find(&p);
	(*hash.find(p))->setIdb();
	delete p;
}

void PredicateTable::getEdbPredicate(unordered_set<index_object>& edb_pred) {
	for (Predicate*p : hash)
		if (p->isEdb())
			edb_pred.insert(p->getIndex());
}
