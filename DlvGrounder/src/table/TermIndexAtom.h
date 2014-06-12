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
	TermIndexAtom(): SimpleIndexAtom(), instantiateIndexMap(false), termToBeIndexed(0),termSetByPreference(false){};
	TermIndexAtom(AtomTable* a): SimpleIndexAtom(a), instantiateIndexMap(false), termToBeIndexed(0),termSetByPreference(false){};
	TermIndexAtom(AtomTable* a, int i): SimpleIndexAtom(a), instantiateIndexMap(false), termToBeIndexed(i),termSetByPreference(true){};
	virtual index_object firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,map_int_int& equal_var,bool& find);
	virtual ~TermIndexAtom(){};
private:
	unordered_map<index_object,AtomTable> indexMap;
	bool instantiateIndexMap;
	unsigned int termToBeIndexed;
	bool termSetByPreference;
	void initializeIndexMap();
	long determineTermToBeIndexed(vec_pair_index_object& bound);
};

class TermIndexAtomMultiMap: public SimpleIndexAtom {
public:
	TermIndexAtomMultiMap(): SimpleIndexAtom(), instantiateIndexMap(false), termToBeIndexed(0),termSetByPreference(false){};
	TermIndexAtomMultiMap(AtomTable* a): SimpleIndexAtom(a), instantiateIndexMap(false), termToBeIndexed(0),termSetByPreference(false){};
	TermIndexAtomMultiMap(AtomTable* a, int i): SimpleIndexAtom(a), instantiateIndexMap(false), termToBeIndexed(i),termSetByPreference(true){};
	virtual index_object firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,map_int_int& equal_var,bool& find);
	virtual ~TermIndexAtomMultiMap(){};
private:
	unordered_multimap<index_object,GenericAtom*> indexMap;
	bool instantiateIndexMap;
	unsigned int termToBeIndexed;
	bool termSetByPreference;
	void initializeIndexMap();
};


#endif /* FIRSTTERMINDEXATOM_H_ */
