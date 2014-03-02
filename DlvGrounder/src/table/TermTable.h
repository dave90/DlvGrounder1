/*
 * TermTable.h
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#ifndef TERMTABLE_H_
#define TERMTABLE_H_


#include "../term/Term.h"

using namespace std;

struct hashTerm {
  size_t operator()(Term* term) const {
	 return term->getIndex();
  }

};

struct equalTerm{
	  bool operator()( Term* t1,  Term* t2)const{
		  return t1->getIndex()==t2->getIndex();
	  }
};

class TermTable {
public:
	TermTable();
	virtual unsigned long addTerm(Term *t)=0;
	virtual void removeTerm(unsigned long index)=0;
	virtual Term* getTerm(unsigned long index)=0;
	virtual long getSize()=0;
	virtual ~TermTable();
};


#endif /* TERMTABLE_H_ */
