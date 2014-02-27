/*
 * TermTable.h
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#ifndef TERMTABLE_H_
#define TERMTABLE_H_


#include "Term.h"

using namespace std;

class TermTable {
public:
	TermTable();
	virtual long addTerm(Term *t)=0;
	virtual void removeTerm(long index)=0;
	virtual Term* getTerm(long index)=0;
	virtual long getSize()=0;
	virtual ~TermTable();
};


#endif /* TERMTABLE_H_ */
