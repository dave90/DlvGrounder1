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

	GenericAtom():atom(nullptr){}
	GenericAtom(Atom * atom){
		this->atom=atom;
	}

	virtual bool isFact(){return true;}
	virtual void setFact(bool isFact){};

	virtual ~GenericAtom(){}
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

typedef vector<pair<unsigned int,index_object> > vec_pair_index_object;
typedef unordered_multimap<unsigned int,unsigned int> map_int_int;

/*
 * Hash function for the class Atom
 */
struct hashAtom {

	size_t operator()(GenericAtom* atomFact) const {
		return HashString::getHashStringFromConfig()->computeHash(atomFact->atom->getNameToHash());
	}
	bool operator()(GenericAtom* t1, GenericAtom* t2) const {
		return *(t1->atom) == *(t2->atom);
	}

};

struct hashAtomResult {
	vec_pair_index_object bind;
	HashString *hash;

	hashAtomResult(){hash=HashString::getHashStringFromConfig();}

	hashAtomResult(vec_pair_index_object &bind){this->bind=bind;hash=HashString::getHashStringFromConfig();}

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
	ResultMatch(vec_pair_index_object &bind){ result=AtomResultTable(0,hashAtomResult(bind));}

};


class IndexAtom {
public:
	IndexAtom(){atoms=0;};
	IndexAtom(AtomTable* a) :atoms(a){};
	;
	/*
	 *  Return id used for the nextMatch
	 */
	virtual index_object firstMatch(vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,bool& find)=0;
	virtual void nextMatch(index_object id,vec_pair_index_object &bind,bool& find)=0;
	virtual ~IndexAtom() {};
protected:
	AtomTable* atoms;
};

class SimpleIndexAtom: public IndexAtom {
public:
	SimpleIndexAtom(AtomTable* a) : IndexAtom(a),counter(0){};
	virtual index_object firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,map_int_int& equal_var,bool& find);
	virtual void nextMatch(index_object id,vec_pair_index_object &bind,bool& find);
	virtual ~SimpleIndexAtom();
protected:
	unordered_map<index_object, ResultMatch*> matches_id;
	unsigned int counter;
	/// Search in the atom table if match
	bool computeFirstMatch(const AtomTable& collection,vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,ResultMatch* rm);
	/// Test the match of bind equal variable
	bool checkEqualVariable(map_int_int& equal_var,Atom *atom);
	virtual bool findIfAFactExists(const AtomTable& collection,vec_pair_index_object& bound, map_int_int& equal_var);
};

class Instances {
public:
	Instances(index_object predicate);

	void addFact(Atom* atom) {
		GenericAtom* atomFact=new GenericAtom(atom);
		if(!facts.count(atomFact))facts.insert(atomFact);else{ delete atom;delete atomFact;}
	};

	// A no fact is true if its truth value is true, otherwise it is undefined, false atoms are not saved.
	bool addNoFact(Atom* atom, bool truth) {
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


	bool isTrue(Atom* atom) {
		GenericAtom *atomUndef=new AtomUndef(atom,0);
		auto it=nofacts.find( atomUndef);
		bool result = (*it)->isFact();
		delete atomUndef;
		return result;
	};


	index_object getPredicate() const {return predicate;}
	void setPredicate(index_object predicate) {this->predicate = predicate;};

	IndexAtom* getIndex() {	return indexAtom;};

	void print(){};

	~Instances();

private:
	index_object predicate;
	IndexAtom* indexAtom;
	AtomTable facts;
	AtomTable nofacts;
};


class InstancesTable {
public:
	void addInstance(index_object i) {
		if(!instanceTable.count(i)){
			Instances* is = new Instances(i);
			instanceTable.insert({i,is});
		}
	};
	// Get term by the index
	Instances* getInstance(index_object i) {
		auto instance = instanceTable.find(i);
		if(instance!=instanceTable.end())
			return instance->second;
		else
			return nullptr;
	};
	// Get size of the table
	long getSize() {return instanceTable.size();};
	void print() {for (auto i : instanceTable)i.second->print();	};
	~InstancesTable();
private:
	unordered_map<index_object,Instances*> instanceTable;
};

#endif /* INSTANCE_H_ */
