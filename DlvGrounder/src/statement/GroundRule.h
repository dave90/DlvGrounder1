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

/// This struct represents a ground atom, composed by a predicate and a generic atom @see GenericAtom
struct GroundAtom{

	index_object predicate;
	GenericAtom* atom;
	bool negative;

	GroundAtom(index_object predicate,GenericAtom* atom): predicate(predicate), atom(atom){negative=false;}
	GroundAtom(index_object predicate,GenericAtom* atom,bool negative): predicate(predicate), atom(atom),negative(negative){}

	GroundAtom(index_object predicate,vector<index_object>& terms): predicate(predicate) {
		atom=new GenericAtom(terms);
		negative=false;
	}


	GroundAtom(index_object predicate,vector<index_object>& terms,bool truth): predicate(predicate) {
		atom=new AtomUndef(terms,truth);
		negative=false;
	}

	/// @brief Equality of ground atoms
	/// @details Two ground atoms are equal if they have the same predicate and their atoms are equal
	inline bool operator==(const GroundAtom& genericAtom) const{
		if(predicate != genericAtom.predicate)
			return false;
		return *atom==*genericAtom.atom;
	}

	/// @brief Hash function for ground atoms
	/// @details The hash in computed using the terms
	inline size_t getHash() const{
		return HashVecInt::getHashVecIntFromConfig()->computeHash(atom->terms);
	}

};

/// @brief This struct implements a comparator for ground atoms
/// @details Ground atoms are compared by their predicate and the resulting hash of their generic atoms
struct atomCompare {
  bool operator() (const GroundAtom* a1, const GroundAtom* a2) const
  {

	  index_object p1=a1->predicate;
	  index_object p2=a2->predicate;
	  if(p1!=p2)
		  return p1<p2;

	  return (a1->getHash() < a2->getHash());
  }
};

///Definition of an ordered set of ground atoms
typedef set<GroundAtom*,atomCompare> OrderedAtomSet;

/**
 * @brief This class represents a ground rule.
 * A ground rule is a rule with ground atoms derived by the grounding of an initial non ground rule.
 *
 * @details In a ground rule the order of the atoms matters for the hash function.
 * Indeed the hash function takes into account the order in which the atoms appear.
 */
class GroundRule {
public:
	///Default constructor
	GroundRule(){};

	///This method adds an atom in the head
	void addInHead(GroundAtom* a){head.insert(a);};
	///This method adds an atom in the body
	void addInBody(GroundAtom* a){body.insert(a);};

	/** @brief Equal-to operator for ground rules
	 *  @details Two ground rules are equal if they have the same ground atoms in the body and in the head regardless the order in which they appear
	 */
	bool operator==(const GroundRule & r);

	///This method returns the size of the head
	unsigned int getSizeHead() const {return head.size();}
	///This method return the size of the body
	unsigned int getSizeBody() const {return body.size();}
	///This method returns an iterator that points to the first atom in the body
	OrderedAtomSet::const_iterator getBeginBody()const{return body.begin();};
	///This method returns an iterator that points to the last atom in the body
	OrderedAtomSet::const_iterator getEndBody()const{return body.end();};
	///This method returns an iterator that points to the first atom in the head
	OrderedAtomSet::const_iterator getBeginHead()const{return head.begin();};
	///This method returns an iterator that points to the last atom in the head
	OrderedAtomSet::const_iterator getEndHead()const{return head.end();};

	/// Remove the atom in the body
	void removeBody(GroundAtom* atom){body.erase(atom);}
	/// Remove the atom in the head
	void removeHead(GroundAtom* atom){head.erase(atom);}

	///Printer method
	void print();

	///Destructor
	virtual ~GroundRule();

private:
	///Ordered set of the ground atoms in the head
	///The order is established comparing the hash of the atoms @see atomCompare
	OrderedAtomSet head;
	///Ordered set of the ground atoms in the head
	///The order is established comparing the hash of the atoms @see atomCompare
	OrderedAtomSet body;
};

#endif /* GROUNDRULE_H_ */
