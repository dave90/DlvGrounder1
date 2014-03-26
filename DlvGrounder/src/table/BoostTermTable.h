/*
 * BoostTermTable.h
 *
 *  Created on: 01/mar/2014
 *      Author: Davide
 */

#ifndef BOOSTTERMTABLE_H_
#define BOOSTTERMTABLE_H_

#include <boost/unordered_set.hpp>

#include <unordered_map>

#include "TermTable.h"

using namespace std;


class BoostTermTable: public TermTable {
public:
	BoostTermTable();
	virtual unsigned long addTerm(Term *t);
	virtual void removeTerm(unsigned long index);
	virtual Term* getTerm(unsigned long index);
	virtual long getSize();
	virtual int getCollision();
	virtual ~BoostTermTable();
private:
	boost::unordered_set<Term*,hashTerm,equalTerm> hash;
};

#endif /* BOOSTTERMTABLE_H_ */
