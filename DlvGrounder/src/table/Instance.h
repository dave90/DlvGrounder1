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
#include <algorithm>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include "HashVecInt.h"
#include "../atom/ClassicalLiteral.h"
#include "PredicateTable.h"

using namespace std;




struct GenericAtom{
	vector<index_object> terms;

	GenericAtom(){}
	GenericAtom(vector<index_object>& t):terms(move(t)){}

	virtual bool isFact(){return true;}
	virtual void setFact(bool isFact){};
	inline bool operator==(const GenericAtom& genericAtom) const{
		if(terms.size()!=genericAtom.terms.size())return false;
		return equal(terms.begin(),terms.end(),genericAtom.terms.begin());

	}

	virtual ~GenericAtom(){}
};

struct AtomUndef : GenericAtom{
	bool fact;

	AtomUndef(vector<index_object>& t,bool f):fact(f){terms=t;}

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
		return HashVecInt::getHashVecIntFromConfig()->computeHash(atomFact->terms);
	}
	 bool operator()(GenericAtom* a1, GenericAtom* a2) const {
		return *a1==*a2;
	}

};

struct hashAtomResult {
	vec_pair_index_object bind;

	hashAtomResult(){}

	hashAtomResult(vec_pair_index_object &bind){this->bind=bind;}

	size_t operator()( GenericAtom* atom) const {
		vector<index_object> terms;
		getTermsBind(atom,terms);
		return HashVecInt::getHashVecIntFromConfig()->computeHash(terms);
	}

	bool operator()( GenericAtom* atom1,  GenericAtom* atom2) const {
		vector<index_object> terms1;
		getTermsBind(atom1,terms1);
		vector<index_object> terms2;
		getTermsBind(atom2,terms2);
		return equal(terms1.begin(),terms1.end(),terms2.begin());
	}

	/// Generate the terms of the atom only with the bind terms
	void getTermsBind( GenericAtom* atom,vector<index_object>& terms) const{
		for(auto t:bind)
			terms.push_back(atom->terms[t.first]);
	}
};

typedef unordered_set<GenericAtom*, hashAtom, hashAtom> AtomTable;
typedef unordered_set<GenericAtom*, hashAtomResult, hashAtomResult> AtomResultTable;


struct ResultMatch {
	mutable AtomResultTable result;
	ResultMatch(vec_pair_index_object &bind){ result=AtomResultTable(0,hashAtomResult(bind),hashAtomResult(bind));}

};


class IndexAtom {
public:
	IndexAtom(AtomTable* facts,AtomTable* nofacts,Predicate *p) :facts(facts), nofacts(nofacts),predicate(p){};
	;
	/*
	 *  Return id used for the nextMatch
	 */
	virtual unsigned int firstMatch(vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,bool& find)=0;
	virtual void nextMatch(unsigned int id,vec_pair_index_object &bind,bool& find)=0;
	virtual ~IndexAtom() {};
protected:
	AtomTable* facts;
	AtomTable* nofacts;
	Predicate* predicate;
};

class SimpleIndexAtom: public IndexAtom {
public:
	SimpleIndexAtom(AtomTable* facts, AtomTable* nofacts,Predicate *p) : IndexAtom(facts,nofacts,p),counter(0){};
	virtual unsigned int firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,map_int_int& equal_var,bool& find);
	virtual void nextMatch(unsigned int id,vec_pair_index_object &bind,bool& find);
	virtual ~SimpleIndexAtom();
protected:
	unordered_map<unsigned int, ResultMatch*> matches_id;
	unsigned int counter;
	/// Search in the atom table if match
	void computeFirstMatch(AtomTable* collection,vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,ResultMatch* rm);
	/// Test the match of bind equal variable
	bool checkEqualVariable(map_int_int& equal_var,GenericAtom *atom);
	///If all terms are bound call findIfAFactExists, otherwise call computeFirstMatch
	bool searchForFirstMatch(AtomTable* table, vec_pair_index_object &bound,vec_pair_index_object &bind,map_int_int& equal_var,ResultMatch* rm);
	///Compute the ground atom from bound terms and equal_var, compute the hash of the atom, and search for that atom efficiently in the collection
	virtual bool findIfAFactExists(AtomTable* collection,vec_pair_index_object& bound, map_int_int& equal_var);
};

class Instances {
public:
	Instances(index_object predicate,PredicateTable *pt);


	bool addFact(vector<index_object>& terms) {
		GenericAtom* atomFact=new GenericAtom(terms);
		if(!facts.insert(atomFact).second){
			delete atomFact;
			return false;
		}
		return true;
	};

	GenericAtom* getGenericAtom(vector<index_object>& terms) {
		GenericAtom* atomFact=new GenericAtom(terms);
		auto it=facts.find(atomFact);
		if(it==facts.end())
			it=nofacts.find(atomFact);

		delete atomFact;
		return *it;
	};

	// A no fact is true if its truth value is true, otherwise it is undefined, false atoms are not saved.
	bool addNoFact(GenericAtom*& atomUndef, bool truth) {
		if(!nofacts.insert(atomUndef).second){
			// Delete the atom duplicate and return atom in nofact table
			GenericAtom* atomToDelete=atomUndef;
			atomUndef=*nofacts.find(atomToDelete);
			delete atomToDelete;
			return false;
		}
		return true;
	};

	void setValue(vector<index_object>& terms, bool truth) {
		GenericAtom *atomUndef=new AtomUndef(terms,truth);
		auto it=nofacts.find( atomUndef);
		(*it)->setFact(truth);
		delete atomUndef;
	};


	bool isTrue(vector<index_object>& terms) {
		GenericAtom *atomUndef=new AtomUndef(terms,0);
		auto it=nofacts.find( atomUndef);
		bool result = (*it)->isFact();
		delete atomUndef;
		return result;
	};


	index_object getPredicate() const {return predicate;}
	void setPredicate(index_object predicate) {this->predicate = predicate;};

	IndexAtom* getIndex() {	return indexAtom;};

	void print(){for(GenericAtom*fact:facts){ClassicalLiteral::print(predicate,fact->terms,false,false); cout<<"."<<endl;}};

	~Instances();

private:
	index_object predicate;
	PredicateTable *predicateTable;
	IndexAtom* indexAtom;
	AtomTable facts;
	AtomTable nofacts;
};


class InstancesTable {
public:
	InstancesTable(PredicateTable *pt):predicateTable(pt){}
	void addInstance(index_object i) {
		if(!instanceTable.count(i)){
			Instances* is = new Instances(i,predicateTable);
			instanceTable.insert({i,is});
		}
	};
	// Get term by the index
	Instances* getInstance(index_object i) {
		auto result= instanceTable.find(i);
		if(result==instanceTable.end()) return nullptr;
		return result->second;
	};
	// Get size of the table
	long getSize() {return instanceTable.size();};
	void print() {for (auto i : instanceTable)i.second->print();	};
	~InstancesTable();
private:
	PredicateTable *predicateTable;
	unordered_map<index_object,Instances*> instanceTable;
};

#endif /* INSTANCE_H_ */
