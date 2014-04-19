/*
 * HashTermTable.h
 *
 *  Created on: 01/mar/2014
 *      Author: Davide
 */

#ifndef HASHTERMTABLE_H_
#define HASHTERMTABLE_H_

#include <unordered_set>
#include <cstring>

#include "TermTable.h"

using namespace std;



class HashTermTable: public TermTable {
public:
	HashTermTable();
	virtual unsigned long addTerm(Term *t);
	virtual void removeTerm(unsigned long index);
	virtual Term* getTerm(unsigned long index);
	virtual long getSize();
	virtual int getCollision();
	virtual ~HashTermTable();
private:
	unordered_set<Term*,hashTerm,hashTerm> hash;
};

#endif /* HASHTERMTABLE_H_ */
