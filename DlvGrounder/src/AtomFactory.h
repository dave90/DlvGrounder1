/*
 * AtomFactory.h
 *
 *  Created on: 27/mar/2014
 *      Author: Davide
 */

#ifndef ATOMFACTORY_H_
#define ATOMFACTORY_H_

#include "table/PredicateTable.h"
#include "table/Instance.h"

class AtomFactory {
public:
	AtomFactory();
	void addPredicate(string name);
	void addGClassicalLiteral(vector<unsigned long> terms,bool hashMinus,bool negative);
	void print();

private:
	PredicateTable predicateTable;
	InstancesTable instancesTable;

	string lastPredicate;
};

#endif /* ATOMFACTORY_H_ */
