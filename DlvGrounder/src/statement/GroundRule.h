/*
 * GroundRule.h
 *
 *  Created on: Jun 26, 2014
 *      Author: david
 */

#ifndef GROUNDRULE_H_
#define GROUNDRULE_H_

#include <set>

#include "../table/Instance.h"
#include "../utility/IndexDefinition.h"

using namespace std;

/// Ground Atom with predicate and generic atom, like in Instance
struct GroundAtom{

	index_object predicate;
	GenericAtom *atom;

	GroundAtom(index_object predicate,GenericAtom* atom):predicate(predicate),atom(atom){}
	GroundAtom(index_object predicate,vector<index_object>& terms):predicate(predicate){
		atom=new GenericAtom(terms);
	}

	inline bool operator==(const GroundAtom& genericAtom) const{
		if(predicate != genericAtom.predicate)return false;
		return *atom==*genericAtom.atom;
	}
	inline size_t getHash() const{
		return HashVecInt::getHashVecIntFromConfig()->computeHash(atom->terms);
	}

};

/// Comparator of Ground Atom with predicate and hash of atoms
struct atomCompare {
  bool operator() (const GroundAtom* a1, const GroundAtom* a2) const
  {

	  index_object p1=a1->predicate;
	  index_object p2=a2->predicate;
	  if(p1!=p2)return p1<p2;

	  return (a1->getHash() < a2->getHash() );
  }
};

typedef set<GroundAtom*,atomCompare> OrderedAtomSet;

/**
 * 	Ground Rule is a rule with ground atom derived after the grounding of non ground rule
 * 	Unlike Rule have ordered atoms for the hash of the GroundeRule
 */
class GroundRule {
public:
	GroundRule(){};

	void addInHead(GroundAtom* a){head.insert(a);};
	void addInBody(GroundAtom* a){body.insert(a);};
	bool operator==(const GroundRule & r);
	unsigned int getSizeHead() const {return head.size();}
	unsigned int getSizeBody() const {return body.size();}
	OrderedAtomSet::const_iterator getBeginBody()const{return body.begin();};
	OrderedAtomSet::const_iterator getEndBody()const{return body.end();};
	OrderedAtomSet::const_iterator getBeginHead()const{return head.begin();};
	OrderedAtomSet::const_iterator getEndHead()const{return head.end();};
	void print();


	virtual ~GroundRule();
private:
	OrderedAtomSet head;
	OrderedAtomSet body;
};

#endif /* GROUNDRULE_H_ */
