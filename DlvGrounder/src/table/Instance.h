/*
 * Instance.h
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "../atom/Atom.h"
#include <iostream>

using namespace std;

struct PairAtomBool {
	Atom* atom;
	mutable bool truth;
};

/*
 * Hash function for the class Atom
 */
struct hashAtom {
	size_t operator()(Atom* atom) const {
		return atom->getIndex();
	}
	size_t operator()(PairAtomBool pb) const {
		return pb.atom->getIndex();
	}
	bool operator()(Atom* t1, Atom* t2) const {
		return t1->getIndex() == t2->getIndex();
	}
	bool operator()(PairAtomBool pb1, PairAtomBool pb2) const {
		return pb1.atom->getIndex() == pb2.atom->getIndex();
	}

};

typedef unordered_set<Atom*, hashAtom, hashAtom> AtomTable;
typedef unordered_set<PairAtomBool, hashAtom, hashAtom> PairAtomBoolTable;
typedef vector<pair<unsigned int,unsigned long> > vec_pair_long;
typedef unordered_multimap<unsigned int,unsigned int> map_int_int;

struct ResultMatch {
	mutable vector<Atom*> result;
};


class IndexAtom {
public:
	IndexAtom(){atoms=0;};
	IndexAtom(AtomTable* a) {atoms = a;}
	;
	/*
	 *  Return id used for the nextMatch
	 */
	virtual unsigned long firstMatch(vec_pair_long &bound,vec_pair_long &bind,map_int_int& equal_var,bool& find)=0;
	virtual void nextMatch(unsigned long id,vec_pair_long &bind,bool& find)=0;
	virtual ~IndexAtom() {};
protected:
	AtomTable* atoms;
};

class SimpleIndexAtom: public IndexAtom {
public:
	SimpleIndexAtom(){};
	SimpleIndexAtom(AtomTable* a) {	atoms = a;};
	virtual unsigned long firstMatch(vec_pair_long &bound, vec_pair_long &bind,map_int_int& equal_var,bool& find);
	virtual void nextMatch(unsigned long id,vec_pair_long &bind,bool& find);
	virtual ~SimpleIndexAtom();
protected:
	unordered_map<unsigned long, ResultMatch*> matches_id;
	bool isPresent(unordered_set<string> &result_string,string result_terms);
	bool computeFirstMatch(AtomTable& collection,vec_pair_long &bound,vec_pair_long &bind,map_int_int& equal_var,bool& find,unsigned long& id,ResultMatch* rm,unordered_set<string> result_string);
};

class Instances {
public:
	Instances(unsigned long predicate);

	inline void addFact(Atom* atom) {
		computeAtomIndex(atom);
		if(!facts.count(atom))facts.insert(atom);else delete atom;
	};

	// A no fact is true if its truth value is true, otherwise it is undefined, false atoms are not saved.
	inline bool addNoFact(Atom* atom, bool truth) {
		computeAtomIndex(atom);
		PairAtomBool p;
		p.atom=atom;
		p.truth=truth;
		if(!nofacts.count(p))nofacts.insert(p);else {delete atom;return false;}
		return true;
	};

	void setValue(Atom* atom, bool truth) {	nofacts.find( { atom, true })->truth = truth;};
	void setValue(unsigned long index, bool truth) {Atom* a;a->setIndex(index);nofacts.find( { a, true })->truth = truth;};
	bool isTrue(Atom* atom) {return nofacts.find( { atom, true })->truth;};
	bool isTrue(unsigned long index) {Atom* a;a->setIndex(index);bool res = nofacts.find( { a, true })->truth;delete (a);return res;}

	unsigned long getPredicate() const {return predicate;}
	void setPredicate(unsigned long predicate) {this->predicate = predicate;};

	IndexAtom* getIndex() {	return indexAtom;};

	void print(){};

	~Instances();

private:
	unsigned long predicate;
	IndexAtom* indexAtom;
	AtomTable facts;
	PairAtomBoolTable nofacts;

	void computeAtomIndex(Atom*& a);
};

/*
 * Hash function for the class Atom
 */
struct hashInstance {
	size_t operator()(Instances* i) const {
		return i->getPredicate();
	}
	bool operator()(Instances *i1, Instances* i2) const {
		return i1->getPredicate() == i2->getPredicate();
	}
};

class InstancesTable {
public:
	inline void addInstance(unsigned long i) {
		Instances* is = new Instances(i);
		if(instanceTable.count(is))
			delete is;
		else
			instanceTable.insert(is);
	};
	// Get term by the index
	inline Instances* getInstance(unsigned long i) {
		Instances* is = new Instances(i);
		auto instance = instanceTable.find(is);
		delete is;
		if(instance!=instanceTable.end())
			return *(instance);
		else
			return nullptr;
	};
	// Get size of the table
	long getSize() {return instanceTable.size();};
	void print() {for (Instances* i : instanceTable)i->print();	};
	~InstancesTable();
private:
	unordered_set<Instances*, hashInstance, hashInstance> instanceTable;
};

#endif /* INSTANCE_H_ */
