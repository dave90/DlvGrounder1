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

struct ResultMatch {
	mutable vector<Atom*> result;
};



struct Tuple_Match{
	///@param a is ground atom
	///@param bind contains pair of index of term to bind and the variable
	Tuple_Match(Atom *a,vec_pair_long bind){
		for(auto b:bind){
			unsigned long variable=b.second;
			unsigned long values=a->getTerm(b.first);
			variable_values.insert({variable,values});
		}
	}
	unordered_map<unsigned long,unsigned long> variable_values;
};

struct Hash_TupleSet{
	Hash_TupleSet(vector<unsigned long> variables):variables(variables){}
	size_t operator()(Tuple_Match* tuple) const {
		vector<unsigned long> values;
		for(unsigned long var:variables)
			values.push_back(tuple->variable_values.find(var)->second);

		return boost::hash_range(values.begin(),values.end());
	}

	bool operator()( Tuple_Match* t1,  Tuple_Match* t2)const{
		for(unsigned long var:variables)
			if(t1->variable_values.find(var)->second!=t2->variable_values.find(var)->second)
				return false;
		return true;
	}
	vector<unsigned long> variables;
};

typedef unordered_set<Tuple_Match*,Hash_TupleSet,Hash_TupleSet> Tuple_Match_Set;

class IndexAtom {
public:
	IndexAtom(){atoms=0;};
	IndexAtom(AtomTable* a) {atoms = a;}
	;
	/*
	 *  Return id used for the nextMatch
	 */
	virtual unsigned long firstMatch(vec_pair_long &bound,vec_pair_long &bind,bool& find)=0;
	virtual void nextMatch(unsigned long id,vec_pair_long &bind,bool& find)=0;
	virtual void match(vec_pair_long &bound,vec_pair_long &bind,Tuple_Match_Set& set)=0;
	virtual ~IndexAtom() {};
protected:
	AtomTable* atoms;
};

class SimpleIndexAtom: public IndexAtom {
public:
	SimpleIndexAtom(){};
	SimpleIndexAtom(AtomTable* a) {	atoms = a;};
	virtual unsigned long firstMatch(vec_pair_long &bound, vec_pair_long &bind,bool& find);
	virtual void nextMatch(unsigned long id,vec_pair_long &bind,bool& find);
	virtual void match(vec_pair_long &bound,vec_pair_long &bind,Tuple_Match_Set& set);
	virtual ~SimpleIndexAtom();
private:
	unordered_map<unsigned long, ResultMatch*> matches_id;
};

class Instances {
public:
	Instances(unsigned long predicate);

	void addFact(Atom* atom) {computeAtomIndex(atom);facts.insert(atom);}
	;
	// A no fact is true if its truth value is true, otherwise it is undefined, false atoms are not saved.
	void addNoFact(Atom* atom, bool truth) {computeAtomIndex(atom);	nofacts.insert( { atom, truth });}
	;

	void setValue(Atom* atom, bool truth) {	nofacts.find( { atom, true })->truth = truth;}
	;
	void setValue(unsigned long index, bool truth) {Atom* a;a->setIndex(index);nofacts.find( { a, true })->truth = truth;}
	;
	bool isTrue(Atom* atom) {return nofacts.find( { atom, true })->truth;}
	;
	bool isTrue(unsigned long index) {Atom* a;a->setIndex(index);bool res = nofacts.find( { a, true })->truth;delete (a);return res;}

	unsigned long getPredicate() const {return predicate;}
	void setPredicate(unsigned long predicate) {this->predicate = predicate;}
	;

	IndexAtom* getIndex() {	return indexAtom;}
	;

	void print();

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
	void addInstance(unsigned long i) {	Instances* is = new Instances(i);instanceTable.insert(is);};
	// Get term by the index
	Instances* getInstance(unsigned long i) {Instances* is = new Instances(i);return *(instanceTable.find(is));};
	// Get size of the table
	long getSize() {return instanceTable.size();};
	void print() {for (Instances* i : instanceTable)i->print();	};
	~InstancesTable();
private:
	unordered_set<Instances*, hashInstance, hashInstance> instanceTable;
};

#endif /* INSTANCE_H_ */
