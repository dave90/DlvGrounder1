/*
 * TermTable.h
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#ifndef TERMTABLE_H_
#define TERMTABLE_H_

/*
 *  TermTable is an abstract class that rappresent a table of all Term in
 *   the logical program
 */

#include "../term/Term.h"

using namespace std;

/*
 * Hash function for the class Term
 */
struct hashTerm {
  size_t operator()(Term* term) const {
	 return term->getIndex();
  }
	  bool operator()( Term* t1,  Term* t2)const{
		  return t1->getIndex()==t2->getIndex();
	  }

};

class TermTable {
public:
	TermTable();
	//Add term in the table
	virtual unsigned long addTerm(Term *t)=0;
	// Remove term in the table
	virtual void removeTerm(unsigned long index)=0;
	// Get term by the index
	virtual Term* getTerm(unsigned long index)=0;
	// Get size of the table
	virtual long getSize()=0;
	// Get the collision of the IdManager
	virtual int getCollision()=0;
	virtual void print()=0;
	virtual ~TermTable();
};


#endif /* TERMTABLE_H_ */
