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

unsigned long PredicateTable::insertPredicate(Predicate* p) {
	pair<unsigned long, bool> resultIdManager=IdsManager::getIndex(IdsManager::PREDICATE_ID_MANAGER, p->getNameToHash());
	unsigned long index=resultIdManager.first;
	if(!resultIdManager.second){
		p->setIndex(index);
		hash.insert(p);
	}

	return index;


}

Predicate* PredicateTable::getPredicate(unsigned long id) {

	Predicate p;
	p.setIndex(id);

	Predicate *p_finded= *hash.find(&p);
	return p_finded;
}

void PredicateTable::setEdb(unsigned long index) {
	Predicate p;
	p.setIndex(index);
	Predicate *tmp=*hash.find(&p);
	tmp->setEdb();
}

void PredicateTable::setIdb(unsigned long index) {
	Predicate p;
	p.setIndex(index);
	Predicate *tmp=*hash.find(&p);
	tmp->setIdb();
}
