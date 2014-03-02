/*
 * MapTermTable.h
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#ifndef MAPTERMTABLE_H_
#define MAPTERMTABLE_H_

#include <map>
#include <string>

#include "TermTable.h"

using namespace std;

class MapTermTable: public TermTable {
public:
	MapTermTable();
	unsigned long addTerm(Term *t);
	void removeTerm(unsigned long index);
	Term* getTerm(unsigned long index);
	virtual long getSize();
	~MapTermTable();
private:
	map<unsigned long,Term*> hash;
	map<string,unsigned long> nameToIndex;
	unsigned long counter;

};

#endif /* MAPTERMTABLE_H_ */
