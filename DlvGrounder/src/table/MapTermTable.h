/*
 * MapTermTable.h
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#ifndef MAPTERMTABLE_H_
#define MAPTERMTABLE_H_

/*
 *  Extends TermTable and use a stl map for storing the terms
 */

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
	//All the term are saved in that map, the key is a term index
	map<unsigned long,Term*> hash;
	// The map save the index of a name of a term, for saving space save only one Term per name
	// SOLVING THE PROBLEM OF A FUNCTION, f(X) != f(23)
	map<string,unsigned long> nameToIndex;
	//counter for assign the index of one term
	unsigned long counter;

};

#endif /* MAPTERMTABLE_H_ */
