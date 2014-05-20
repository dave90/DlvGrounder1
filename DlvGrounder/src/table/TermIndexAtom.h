/*
 * FirstTermIndexAtom.h
 *
 *  Created on: 17/mag/2014
 *      Author: Jessica
 */

#ifndef FIRSTTERMINDEXATOM_H_
#define FIRSTTERMINDEXATOM_H_

#include "Instance.h"

class TermIndexAtom: public SimpleIndexAtom {
public:
	TermIndexAtom(){ atoms=0; instantiateIndexMap=false; termToBeIndexed=0;};
	TermIndexAtom(AtomTable* a) { atoms=a; instantiateIndexMap=false; termToBeIndexed=0;};
	TermIndexAtom(AtomTable* a, int i) { atoms=a; instantiateIndexMap=false; termToBeIndexed=i;};
	virtual unsigned long firstMatch(vec_pair_long &bound, vec_pair_long &bind,map_int_int& equal_var,bool& find);
	virtual ~TermIndexAtom();
private:
	unordered_map<unsigned long,AtomTable*> indexMap;
	bool instantiateIndexMap;
	unsigned int termToBeIndexed;
	void inizializeIndexMap();
};

#endif /* FIRSTTERMINDEXATOM_H_ */
