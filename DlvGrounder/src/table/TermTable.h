/*
 * TermTable.h
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#ifndef TERMTABLE_H_
#define TERMTABLE_H_


#include "../term/Term.h"
#include "../utility/Config.h"

using namespace std;

/**
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

/*
 *  TermTable is an abstract class that represent a table of all Term in
 *  the logical program
 *
 *  TermTable is a Singleton
 */
class TermTable {
public:
	TermTable(){};
	///Add term in the table and return the index
	virtual index_object addTerm(Term *t)=0;
	///Add term in the table with the hash string s and return the index
	virtual index_object addTerm(Term *t,string &s)=0;
	///Add term in the table with the hash integer s and return the index
	virtual index_object addTerm(Term *t,unsigned int &s)=0;
	/// Get term by the index
	virtual Term* getTerm(index_object index)=0;
	/// Get size of the table
	virtual unsigned int getSize()=0;
	/// Get the number of collision of the IdManager
	virtual int getCollision()=0;
	virtual ~TermTable(){};

	// Method for fetch the instance of singleton
    // If termTable is null instance the table based on parameter in the configuration
	static TermTable* getInstance();
protected:
	// Instance of the singleton
	static TermTable* termTable;
};


#endif /* TERMTABLE_H_ */
