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
	virtual index_object addTerm(Term *t);
	virtual index_object addTerm(Term *t,string &s);
	virtual index_object addTerm(Term *t,unsigned int &s);
	virtual void removeTerm(index_object index);
	virtual Term* getTerm(index_object index);
	virtual unsigned int getSize();
	virtual int getCollision();
	virtual ~HashTermTable();

	static TermTable* getInstance(){return new HashTermTable;}
	HashTermTable(){}
protected:

	unordered_set<Term*,hashTerm,hashTerm> hash;
};


#endif /* HASHTERMTABLE_H_ */
