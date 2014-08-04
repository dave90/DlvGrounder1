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
	virtual Term* getTerm(index_object index);
	virtual unsigned int getSize();
	virtual int getCollision();
	virtual ~HashTermTable();

	/// Create a new BoostTermTable, call only in TermTable
	static TermTable* getInstance(){return new HashTermTable;}
protected:
	///Protected constructor
	HashTermTable(){}

	unordered_set<Term*,hashTerm,hashTerm> hash;
private:
	/// Insert the term in the table with the specified id if insert is true, else delete
	void insertTermByIndex(index_object &id,bool insert, Term* t);
};


#endif /* HASHTERMTABLE_H_ */
