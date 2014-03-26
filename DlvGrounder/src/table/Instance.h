/*
 * Instance.h
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <unordered_set>
#include <vector>
#include "../atom/Atom.h"
#include "IdManager.h"
using namespace std;

struct PairAtomBool{
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

};

/*
 * Equal function for the class Atom
 */
struct equalAtom{
	  bool operator()( Atom* t1,  Atom* t2)const{
		  return t1->getIndex()==t2->getIndex();
	  }
	  bool operator()( PairAtomBool pb1, PairAtomBool pb2)const{
	 		  return pb1.atom->getIndex()==pb2.atom->getIndex();
	  }
};

typedef unordered_set<Atom*, hashAtom, equalAtom> AtomTable;
typedef unordered_set<PairAtomBool, hashAtom, equalAtom> PairAtomBoolTable;

class IndexAtom {
	public:
		IndexAtom(AtomTable*);
		/* 	These methods given a vector of terms indices, and a vector of values for that terms returns the first atom (as vector of
			indices of his terms) that satisfy the match.  */
		virtual vector<unsigned long> firstMatch(vector<unsigned long> termsIndex, vector<long> values)=0;
		/* 	These methods given a vector of terms indices, and a vector of values for that terms returns the next atom (as vector of
			indices of his terms) that satisfy the match.
			It has to be invoked after fistMatch. */
		virtual vector<unsigned long> nextMatch()=0;
		/* 	These methods given a vector of terms indices, and a vector of values for that terms returns all the atoms (as vector of
			indices of terms) that satisfy the match. */
		virtual vector<unsigned long> match(vector<unsigned long> termsIndex, vector<long> values)=0;
		virtual ~IndexAtom();
	private:
		AtomTable* atoms;

};

class Instances {
public:
	Instances(unsigned long predicate);

	void addFact(Atom* atom) { computeAtomIndex(atom); facts.insert(atom); };
	// A no fact is true if its truth value is true, otherwise it is undefined, false atoms are not saved.
	void addNoFact(Atom* atom, bool truth) { computeAtomIndex(atom); nofacts.insert({atom,truth}); };

	void setValue(Atom* atom, bool truth) { nofacts.find({atom,true})->truth=truth; };
	void setValue(unsigned long index, bool truth) { Atom* a; a->setIndex(index);  nofacts.find({a,true})->truth=truth; };
	bool isTrue(Atom* atom) { return nofacts.find({atom,true})->truth;};
	bool isTrue(unsigned long index) { Atom* a; a->setIndex(index);  bool res=nofacts.find({a, true })->truth; delete(a); return res;}

	unsigned long getPredicate() const {return predicate;}
	void setPredicate(unsigned long predicate) {this->predicate = predicate;};

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
	  size_t operator()(Instances i) const {
		 return i.getPredicate();
	  }
};

/*
 * Equal function for the class Atom
 */
struct equalInstance {
	  bool operator()( Instances i1,  Instances i2)const{
		  return i1.getPredicate()==i2.getPredicate();
	  }
};

class InstancesTable{
public:
	void addInstance(unsigned long i) { Instances is(i); instanceTable.insert(is); };
	// Get term by the index
	const Instances* getInstance(unsigned long i) {Instances is(i); return &(*instanceTable.find(is)); };
	// Get size of the table
	long getSize() {return instanceTable.size();};
private:
	unordered_set<Instances, hashInstance, equalInstance> instanceTable;
};


#endif /* INSTANCE_H_ */
