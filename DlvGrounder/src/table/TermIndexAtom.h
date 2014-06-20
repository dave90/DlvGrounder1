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
	TermIndexAtom(AtomTable* facts, AtomTable* nofacts,Predicate *p) : SimpleIndexAtom(facts,nofacts,p), instantiateIndexMap(false), termToBeIndexed(0), termSetByPreference(false){};
	TermIndexAtom(AtomTable* facts, AtomTable* nofacts, int i,Predicate *p): SimpleIndexAtom(facts,nofacts,p), instantiateIndexMap(false), termToBeIndexed(i), termSetByPreference(true){};
	virtual unsigned int firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,map_int_int& equal_var,bool& find);
	virtual ~TermIndexAtom(){};
private:
	unordered_map<index_object,AtomTable> factsIndexMap;
	unordered_map<index_object,AtomTable> nofactsIndexMap;
	bool instantiateIndexMap;
	unsigned int termToBeIndexed;
	bool termSetByPreference;
	void initializeIndexMap();
	void determineTermToBeIndexed(vec_pair_index_object& bound);
};

class TermIndexAtomMultiMap: public SimpleIndexAtom {
public:
	TermIndexAtomMultiMap(AtomTable* facts, AtomTable* nofacts,Predicate *p) : SimpleIndexAtom(facts,nofacts,p), instantiateIndexMap(false), termToBeIndexed(0),termSetByPreference(false){};
	TermIndexAtomMultiMap(AtomTable* facts, AtomTable* nofacts, int i,Predicate *p): SimpleIndexAtom(facts,nofacts,p), instantiateIndexMap(false), termToBeIndexed(i),termSetByPreference(true){};
	virtual unsigned int firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,map_int_int& equal_var,bool& find);
	virtual ~TermIndexAtomMultiMap(){};
private:
	unordered_multimap<index_object,GenericAtom*> indexMap;
	bool instantiateIndexMap;
	unsigned int termToBeIndexed;
	bool termSetByPreference;
	void initializeIndexMap();
	void determineTermToBeIndexed(vec_pair_index_object& bound);
};


#endif /* FIRSTTERMINDEXATOM_H_ */
