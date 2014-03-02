/*
 * HashTermTable.h
 *
 *  Created on: 01/mar/2014
 *      Author: Davide
 */

#ifndef HASHTERMTABLE_H_
#define HASHTERMTABLE_H_

#include <map>
#include <unordered_set>
#include <unordered_map>
#include <cstring>

#include "TermTable.h"
#include "../term/VariableTerm.h"

using namespace std;



class HashTermTable: public TermTable {
public:
	HashTermTable();
	virtual unsigned long addTerm(Term *t);
	virtual void removeTerm(unsigned long index);
	virtual Term* getTerm(unsigned long index);
	virtual long getSize();
	virtual ~HashTermTable();
private:
	unordered_set<Term*,hashTerm,equalTerm> hash;
	unordered_multimap<string,unsigned long> nameToIndex;
	unsigned long counter;
};

#endif /* HASHTERMTABLE_H_ */
