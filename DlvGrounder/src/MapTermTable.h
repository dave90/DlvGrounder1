/*
 * MapTermTable.h
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#ifndef MAPTERMTABLE_H_
#define MAPTERMTABLE_H_

#include <map>
#include "TermTable.h"

using namespace std;

class MapTermTable: public TermTable {
public:
	MapTermTable();
	long addTerm(Term *t);
	void removeTerm(long index);
	Term* getTerm(long index);
	long getSize();
private:
	map<long,Term*> hash;
	long counter;
};

#endif /* MAPTERMTABLE_H_ */
