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
#include "../atom/ClassicalLiteral.h"
#include <iostream>

#include <boost/lexical_cast.hpp>


using namespace std;




struct GenericAtom{
	Atom *atom;

	GenericAtom(){}
	GenericAtom(Atom * atom){
		this->atom=atom;
	}

	virtual bool isFact(){return true;}
	virtual void setFact(bool isFact){};
};

struct AtomUndef : GenericAtom{
	bool fact;

	AtomUndef(Atom * atom,bool fact){
		this->atom=atom;
		this->fact=fact;
	}

	bool isFact(){return fact;}
	void setFact(bool fact){this->fact=fact;};
};

typedef vector<pair<unsigned int,unsigned long> > vec_pair_long;
typedef unordered_multimap<unsigned int,unsigned int> map_int_int;

/*
 * Hash function for the class Atom
 */
struct hashAtom {
	size_t operator()(GenericAtom* atomFact) const {
		return atomFact->atom->getIndex();
	}
	bool operator()(GenericAtom* t1, GenericAtom* t2) const {
		return t1->atom->getIndex() == t2->atom->getIndex();
	}

};

struct hashAtomResult {
	vec_pair_long bind;
	HashString *hash;

	hashAtomResult(vec_pair_long &bind){this->bind=bind;hash=HashString::getHashStringFromConfig();}

	size_t operator()(Atom* atom) const {
		return hash->computeHash(getString(atom));
	}

	bool operator()(Atom* a1, Atom* a2) const {
		return getString(a1).compare(getString(a2)) == 0;
	}

	/// Generate the string of the atom only with the bind terms
	string getString(Atom * atom) const{
		string result_terms="";
		for (auto i : bind){
			result_terms+=boost::lexical_cast<string>(atom->getTerm(i.first))+"*";
		}
		return result_terms;
	}
};

typedef unordered_set<GenericAtom*, hashAtom, hashAtom> AtomTable;
typedef unordered_set<Atom*, hashAtomResult, hashAtomResult> AtomResultTable;


struct ResultMatch {
	mutable AtomResultTable result;
	ResultMatch(vec_pair_long &bind):result(0,hashAtomResult(bind),hashAtomResult(bind)){};

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
	bool computeFirstMatch(const AtomTable& collection,vec_pair_long &bound,vec_pair_long &bind,map_int_int& equal_var,ResultMatch* rm);
	/// Test the match of bind equal variable
	bool checkEqualVariable(map_int_int& equal_var,Atom *atom);
	long findIfAFactExists(vec_pair_long& bound, map_int_int& equal_var);
};

class Instances {
public:
	Instances(unsigned long predicate);

	void addFact(Atom* atom) {
		computeAtomIndex(atom);
		GenericAtom* atomFact=new GenericAtom(atom);
		if(!facts.count(atomFact))facts.insert(atomFact);else{ delete atom;delete atomFact;}
	};

	// A no fact is true if its truth value is true, otherwise it is undefined, false atoms are not saved.
	bool addNoFact(Atom* atom, bool truth) {
		computeAtomIndex(atom);
		GenericAtom* atomUndef=new AtomUndef(atom,truth);
		if(!nofacts.count(atomUndef))nofacts.insert(atomUndef);else {delete atom;delete atomUndef;return false;}
		return true;
	};

	void setValue(Atom* atom, bool truth) {
		GenericAtom *atomUndef=new AtomUndef(atom,truth);
		auto it=nofacts.find( atomUndef);
		(*it)->setFact(truth);
		delete atomUndef;
	};

	void setValue(unsigned long index, bool truth) {
		Atom* a=new ClassicalLiteral();
		a->setIndex(index);
		GenericAtom *atomUndef=new AtomUndef(a,truth);
		auto it=nofacts.find( atomUndef);
		(*it)->setFact(truth);
		delete a;
		delete atomUndef;
	};

	bool isTrue(Atom* atom) {
		GenericAtom *atomUndef=new AtomUndef(atom,0);
		auto it=nofacts.find( atomUndef);
		return (*it)->isFact();
	};

	bool isTrue(unsigned long index) {
		Atom* a=new ClassicalLiteral();
		a->setIndex(index);
		GenericAtom *atomUndef=new AtomUndef(a,0);
		auto it=nofacts.find( atomUndef);
		delete a;
		delete atomUndef;
		return (*it)->isFact();
	}

	unsigned long getPredicate() const {return predicate;}
	void setPredicate(unsigned long predicate) {this->predicate = predicate;};

	IndexAtom* getIndex() {	return indexAtom;};

	void print(){};

	~Instances();

private:
	unsigned long predicate;
	IndexAtom* indexAtom;
	AtomTable facts;
	AtomTable nofacts;

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
	void addInstance(unsigned long i) {
		Instances* is = new Instances(i);
		if(instanceTable.count(is))
			delete is;
		else
			instanceTable.insert(is);
	};
	// Get term by the index
	Instances* getInstance(unsigned long i) {
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
