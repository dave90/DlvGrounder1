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
using namespace std;

typedef unordered_set<Atom*> AtomTable;
typedef unordered_set<pair<Atom*, bool>> PairAtomBoolTable;

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
	private:
		AtomTable* atoms;

};

class Instance {
public:
	Instance(unsigned long predicate);

	void addFact(Atom* atom) { facts.insert(atom); };
	// A no fact is true if its truth value is true, otherwise it is undefined, false atoms are not saved.
	void addNoFact(Atom* atom, bool truth) { nofacts.insert({atom,truth}); };
	void setValue(Atom* atom, bool truth) { nofacts.find({atom,true})->second=truth; }; //FIXME
	void setValue(unsigned long index, bool truth) { Atom* a(index);  nofacts.find({a,true})->second=truth;};
	bool isTrue(Atom* atom) { return nofacts.find({atom,true})->second;};
	bool isTrue(unsigned long index) { Atom* a =new ClassicalLiteral(index); bool res=nofacts.find({a, true })->second; delete(a); return res;}

	unsigned long getPredicate() const {return predicate;}
	void setPredicate(unsigned long predicate) {this->predicate = predicate;};

	~Instance();

private:
	unsigned long predicate;
	IndexAtom* indexAtom;
	AtomTable facts;
	PairAtomBoolTable nofacts;
};


class InstanceTable{
public:
	void addInstance(unsigned long i) { Instance is(i); instanceTable.insert(is); };
	// Get term by the index
	Instance* getInstance(unsigned long i) {Instance is(i); return &instanceTable.find(is); };
	// Get size of the table
	long getSize() {return instanceTable.size();};
private:
	unordered_set<Instance> instanceTable;
};


#endif /* INSTANCE_H_ */
