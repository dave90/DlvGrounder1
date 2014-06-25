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
	virtual index_object addTerm(Term *t);
	virtual index_object addTerm(Term *t,string& s);
	virtual index_object addTerm(Term *t,unsigned int &s);
	virtual void removeTerm(index_object index);
	virtual Term* getTerm(index_object index);
	virtual unsigned int getSize();
	virtual int getCollision();
	virtual ~BoostTermTable();
	static TermTable* getInstance(){return new BoostTermTable;}


protected:


	BoostTermTable(){}
	boost::unordered_set<Term*,hashTerm,hashTerm> hash;
};

#endif /* BOOSTTERMTABLE_H_ */
