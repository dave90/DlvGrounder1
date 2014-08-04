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


/**
 *  Concrete class of TermTable, implemented with boost unordered set
 */
class BoostTermTable: public TermTable {
public:
	virtual index_object addTerm(Term *t);
	virtual index_object addTerm(Term *t,string& s);
	virtual index_object addTerm(Term *t,unsigned int &s);
	virtual Term* getTerm(index_object index);
	virtual unsigned int getSize();
	virtual int getCollision();
	/// Create a new BoostTermTable, call only in TermTable
	static TermTable* getInstance(){return new BoostTermTable;}
	virtual ~BoostTermTable();


protected:
	///Protected constructor
	BoostTermTable(){}

	boost::unordered_set<Term*,hashTerm,hashTerm> hash;

private:
	/// Insert the term in the table with the specified id if insert is true, else delete
	void insertTermByIndex(index_object &id,bool insert, Term* t);
};

#endif /* BOOSTTERMTABLE_H_ */
