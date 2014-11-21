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

class IndexAtom;

///This struct implements a generic atom composed by just its terms
struct GenericAtom : Hashable{
	vector<Term*> terms;

	GenericAtom(){}
	GenericAtom(const vector<Term*>& t): terms(move(t)){}

	virtual bool isFact() {return true;}
	virtual void setFact(bool isFact){};

	virtual size_t hash(){
		return HashVecInt::getHashVecIntFromConfig()->computeHashTerm(terms);
	}

	inline bool operator==(const GenericAtom& genericAtom) const{
		if(terms.size()!=genericAtom.terms.size())
			return false;
		for(unsigned i=0;i<terms.size();i++)
			if(terms[i]->getIndex()!=genericAtom.terms[i]->getIndex())
				return false;
		return true;
	}

	virtual ~GenericAtom(){}
};

///This struct implements an undefined atom extending GenericAtom (@see GenericAtom) adding a boolean value to determine whether it is true or not.
///An undefined atoms is an atom derived during the grounding process.
struct AtomUndef : public GenericAtom{
	bool fact;

	AtomUndef(vector<Term*>& t,bool f): GenericAtom(t), fact(f) {}

	bool isFact(){return fact;}
	void setFact(bool fact){this->fact=fact;};
};

///A vector of pair of unsigned integer values and index_object values @see IndexDefinition
typedef vector<pair<unsigned int,index_object>> vec_pair_index_object;
///An unordered map with unsigned integers as keys and values
typedef unordered_multimap<unsigned int,unsigned int> map_int_int;


/* @brief This struct contains the bind variables of an atom.
 *
 * @details During grounding process the variables of an atom are classified as bind or bound.
 * Bind variables are the ones that have no assignment yet, while the bound ones have an assignment.
 * This struct is useful in order to compare and hash atoms just by their bind variables.
 */

struct hashAtomResult {
	vector<unsigned int> bind;

	hashAtomResult(){}

	hashAtomResult(vector<unsigned int> &bind){this->bind=bind;}

	///Hash function built over bind variables
	size_t operator()(GenericAtom* atom) const {
		vector<Term*> terms;
		getTermsBind(atom,terms);
		return HashVecInt::getHashVecIntFromConfig()->computeHashTerm(terms);
	}

	///Equality comparison of two generic atoms over their bind variables
	bool operator()(GenericAtom* atom1,  GenericAtom* atom2) const {
		if(atom1->terms.size()!=atom2->terms.size())return false;
		vector<Term*> terms1;
		getTermsBind(atom1,terms1);
		vector<Term*> terms2;
		getTermsBind(atom2,terms2);
		for(unsigned i=0;i<atom1->terms.size();i++)
			if(atom1->terms[i]->getIndex()!=atom2->terms[i]->getIndex())
				return false;
		return true;
	}

	///This method filters the bind variables of a generic atoms and puts them in the vector given as parameters
	void getTermsBind( GenericAtom* atom, vector<Term*>& terms) const{
		for(auto t:bind)
			terms.push_back(atom->terms[t]);
	}
};

///An unordered set of generic atoms by hashAtom @see hashAtom
typedef unordered_set<GenericAtom*, HashForTable<GenericAtom>, HashForTable<GenericAtom>> AtomTable;
///An unordered map of generic atoms defined by hashResultAtom @see hashResultAtom
/// A map contain the ground atom that match anda vector contains the corrispective variable bind and the value
typedef unordered_map<GenericAtom*,map_term_term, hashAtomResult, hashAtomResult> AtomResultTable;


///This struct implements an AtomResultTable (@see AtomResultTable) that represents a set of possible assignments for bind variables
struct ResultMatch {
	AtomResultTable result;
	ResultMatch(vector<unsigned int> &bind): result(AtomResultTable(0,hashAtomResult(bind),hashAtomResult(bind))){};
	/// Insert the current atom in the table if match with the template
	/// If insert the atom return true else false
	bool insert(GenericAtom* atom,Atom* templateAtom,map_term_term& currentAssignment){
		// Check if the match atom with template, if match put in result table
		map_term_term variableBindFinded;
		if (match(atom,templateAtom,currentAssignment,variableBindFinded)){
			result.insert({atom,variableBindFinded});
			return true;

		}
		return false;
	}

	/// Pop last atom in the table and put the assignment in the current assignment
	/// If the table is empty return false, else true
	bool pop(map_term_term& currentAssignment){
		if(result.size()==0)return false;
		auto it_last_atom=result.begin();
		for(auto resultValue:it_last_atom->second)currentAssignment.insert(resultValue);
		result.erase(it_last_atom);
		return true;
	}

	/// Return true if the genericAtom match with the template atom and insert in assignment the value of variable
	/// Two atom match if a constant term are equal.
	/// @param varAssignment map of assignment of the variable. If templateAtom have variable term put in
	/// varAssignment the ID of variable with the relative constant term of the genericAtom
	/// nextAssignment contains the new value of variable derived
	bool match(GenericAtom *genericAtom,Atom *templateAtom,map_term_term& currentAssignment,map_term_term& nextAssignment);
};


/**
 * This class is an abstract base class that models a general indexing strategy of a predicate's instances.
 * It is builded according to the Strategy GOF pattern.
 */

class IndexAtom {
public:
	//Constructor for all the fields
	IndexAtom(AtomTable* facts, AtomTable* nofacts, AtomTable* delta, Predicate *p) : facts(facts), nofacts(nofacts), delta(delta), predicate(p) {};
	///This method implementation is demanded to sub-classes.
	///It have to find all the matching atoms and return just the first of those.
	///The returned integer will be used to get the other ones through nextMatch method @see nextMatch
	virtual unsigned int firstMatch(bool searchInDelta, Atom *templateAtom, map_term_term& currentAssignment, bool& find)=0;
	///This method implementation is demanded to sub-classes.
	///It is used to get the further matching atoms one by one each time it is invoked.
	virtual void nextMatch(unsigned int id, map_term_term& currentAssignment, bool& find)=0;
	///This method implementation is demanded to sub-classes.
	/// It have to find if exist the templateAtom, that have to be ground
	virtual void findIfExist(bool searchInDelta,Atom *templateAtom, bool& find,bool& isUndef)=0;
	///This method implementation is demanded to sub-classes.
	///It determine whether the given atom is present in the specified table.
	///@param table An integer that can be one among the constants FACTS, NOFACTS or DELTA
	///@parm atom The atom to look for
	virtual bool count(int table, GenericAtom*& atom)=0;
	///This method implementation is demanded to sub-classes.
	///It present, it finds the given atom in the specified table.
	///@param table An integer that can be one among the constants FACTS, NOFACTS or DELTA
	///@parm atom The atom to look for
	virtual void find(int table, GenericAtom*& atom)=0;
	///This method implementation is demanded to sub-classes.
	///It is used to update the delta table for recursive predicates.
	virtual void updateDelta(AtomTable* nextDelta)=0;
	///Destructor
	virtual ~IndexAtom() {};
protected:
	//A pointer to the set of facts
	AtomTable* facts;
	//A pointer to the set of no facts, that are undefined atoms
	AtomTable* nofacts;
	//A pointer to the set of no facts derived in the previous iteration, that are undefined atoms
	AtomTable* delta;
	///The predicate
	Predicate* predicate;

};

/**
 * This class represents the instances (the facts and the no facts) of a predicate
 */

class Instances {
public:
	///Constructor
	Instances(Predicate* predicate): predicate(predicate),indexAtom(0) {
		for(unsigned int i=0;i<4;i++)
			tables.push_back(AtomTable());
		this->setIndexAtom();
	}

	///Getter for the predicate
	Predicate* getPredicate() const {return predicate;}

	///Getter for the IndexAtom
	IndexAtom* getIndex() {return indexAtom;}

	GenericAtom* getGenericAtom(vector<Term*>& terms);

	///This method updates a no facts truth value in no-facts, delta and nextDelta table
	void setValue(vector<Term*>& terms, bool truth);

	///This method determines whether a no fact is true
	bool isTrue(vector<Term*>& terms);

	inline bool addFact(vector<Term*>& terms) {
		// If the atom is not present as fact, it is added.
		// The temporary atom duplicate is deleted and the inserted atom is assigned.
		GenericAtom* atomFact=new GenericAtom(terms);
		if(!tables[Instances::FACTS].insert(atomFact).second){
			indexAtom->find(Instances::FACTS,atomFact);
			return false;
		}
		return true;
	}

	///This method adds a no facts to the no facts table.
	///Its truth value can be true or undefined, if it false it is not stored at all
	bool addNoFact(GenericAtom*& atomUndef,bool& updated);

	///This method adds a no facts to the delta table if it is yet not present in the facts, no facts and delta tables.
	///Its truth value can be true or undefined, if it false it is not stored at all.
	bool addDelta(GenericAtom*& atomUndef,bool& updated);

	///This method adds a no facts to the next delta table if it is yet not present in the facts, no facts, delta and next delta tables.
	///Its truth value can be true or undefined, if it false it is not stored at all.
	bool addNextDelta(GenericAtom*& atomUndef,bool& updated);

	///This method moves the content of the delta table in the no facts table,
	///and then moves the content of the next delta table in the delta table.
	void moveNextDeltaInDelta();

	bool findIn(unsigned int table, GenericAtom*& atomUndef, bool& updated);

	///Printer method
	inline void print(){for(GenericAtom*fact:tables[0]){ClassicalLiteral::print(predicate,fact->terms,false,false); cout<<"."<<endl;}}

	///Destructor
	~Instances();

	static const unsigned int FACTS=0;
	static const unsigned int NOFACTS=1;
	static const unsigned int DELTA=2;
	static const unsigned int NEXTDELTA=3;

private:
	///The predicate
	Predicate* predicate;
	///The IndexAtom to set the indexing strategy
	IndexAtom* indexAtom;
	///The vector of tables: Facts, No-Facts, Delta, NextDelta
	vector<AtomTable> tables;

	///This method configures the indexing strategy
	void setIndexAtom();

};

/**
 * This class stores the instances for all the predicate.
 */

class InstancesTable {
public:
	///Constructor
	InstancesTable(){}

	///This method adds an Instance for a predicate
	void addInstance(Predicate* p) {
		if(!instanceTable.count(p->getIndex())){
			Instances* is = new Instances(p);
			instanceTable.insert({p->getIndex(),is});
		}
	};

	///Getter for the instances of a predicate
	Instances* getInstance(Predicate* p) {
		auto result= instanceTable.find(p->getIndex());
		if(result==instanceTable.end()) return nullptr;
		return result->second;
	};

	///Getter for the instances of a predicate index
	Instances* getInstanceIndex(index_object p) {
		auto result= instanceTable.find(p);
		if(result==instanceTable.end()) return nullptr;
		return result->second;
	};

	///This method return the size of the map of instances
	unsigned int getSize() {return instanceTable.size();};

	///Printer method
	void print() {for (auto i : instanceTable)i.second->print();	};

	///Destructor
	~InstancesTable();
private:

	///The map that stores all the Instances
	unordered_map<index_object,Instances*> instanceTable;
};

/**
 * @brief This class is a basic and simple implementation of IndexAtom (@see IndexAtom)
 * @details Searching for match is performed over the whole tables of facts and non facts with a linear scan.
 */
class SimpleIndexAtom: public IndexAtom {
public:
	///Constructor
	SimpleIndexAtom(AtomTable* facts, AtomTable* nofacts, AtomTable* delta, Predicate *p) : IndexAtom(facts,nofacts,delta,p), counter(0), templateAtom(0), currentAssignment(0){};
	///Virtual method implementation
	virtual unsigned int firstMatch(bool searchInDelta, Atom *templateAtom, map_term_term& currentAssignment, bool& find);
	///Virtual method implementation
	virtual void nextMatch(unsigned int id,map_term_term& currentAssignment, bool& find);
	///Virtual method implementation
	virtual void findIfExist(bool searchInDelta,Atom *templateAtom, bool& find,bool& isUndef);
	///Virtual method implementation
	virtual bool count(int table,GenericAtom*& atom);
	///Virtual method implementation
	virtual void find(int table,GenericAtom*& atom);
	///Virtual method implementation
	virtual void updateDelta(AtomTable* nextDelta){};
	///Destructor
	virtual ~SimpleIndexAtom() {};
protected:
	///The unordered map used to store the integer identifiers returned by the firstMach method
	unordered_map<unsigned int, ResultMatch*> matches_id;
	///The counter used to assign the integer identifiers returned by the firstMach method
	unsigned int counter;
	///Current templateAtom passed in firstMatch
	Atom *templateAtom;
	///Current assignment passed in firstMatch
	map_term_term* currentAssignment;

	///This method given an AtomTable computes the matching facts and nofacts and returns the first one of those
	void computeFirstMatch(AtomTable* collection,ResultMatch* rm);
	///This method invokes findIfAFactExists method if all the variables are bound, otherwise invokes the computeFirstMatch method
	bool searchForFirstMatch(AtomTable* table,ResultMatch* rm);
	///This method builds a ground atom using the bound variables , checks if it is true and return if is undefined or a fact
	virtual bool findIfExists(AtomTable* collection,bool& isUndef);
	///This method builds a ground atom using the bound variables and checks if it is true
	virtual bool findIfExists(AtomTable* collection);
};

/**
 * @brief This class is a more advanced implementation of IndexAtom (@see IndexAtom)
 * @details Searching for match is performed over a tables of facts and no facts indexed by a single term, namely the indexing term.
 * Instead of performing the search among the whole tables it is performed among the facts and the no facts, collectively the instances,
 * that match for that term.
 * Here is used as data structure an unordered map whose keys represent the possible values of the indexing term,
 * that are mapped to unordered sets of instances in which that term appears with that value.
 * This implementation is sibling to the one with multimaps @see SingleTermIndexMultiMap,
 * since the two are implemented to be independent.
 */

class SingleTermIndexAtom: public SimpleIndexAtom {
public:
	/**
	 * Constructor
	 * @param facts An AtomTable of facts
	 * @param nofacts An AtomTable of no facts
	 * @param delta An AtomTable of no facts containing the result of the previous grounding iteration
	 * @param predicate The predicate corresponding to the facts and the no facts.
	 */
	SingleTermIndexAtom(AtomTable* facts, AtomTable* nofacts,AtomTable* delta,Predicate *p) : SimpleIndexAtom(facts,nofacts,delta,p), instantiateIndexMaps(false), positionOfIndexing(0), positionOfIndexingSetByUser(false){};
	///Overload of firstMatch method
	virtual unsigned int firstMatch(bool searchInDelta, Atom *templateAtom, map_term_term& currentAssignment, bool& find);
	///Virtual method implementation
	virtual void updateDelta(AtomTable* nextDelta);
	///Virtual method implementation
	virtual bool count(int table,GenericAtom*& atom);
	///Virtual method implementation
	virtual void find(int table,GenericAtom*& atom);
	///Destructor
	~SingleTermIndexAtom(){};

private:
	///Data structure for indexed facts
	unordered_map<index_object,AtomTable> factsIndexMap;
	///Data structure for indexed no facts
	unordered_map<index_object,AtomTable> nofactsIndexMap;
	///Data structure for indexed delta
	unordered_map<index_object,AtomTable> deltaIndexMap;
	///Determine whether the indexing has been filled in
	bool instantiateIndexMaps;
	///The position of the indexing term
	unsigned int positionOfIndexing;
	///Determine whether the indexing term has been established from command line arguments by the user
	bool positionOfIndexingSetByUser;

	///This method fills in the indexing data structures
	void initializeIndexMaps();

	/// This method carry out the indexing strategy, determining the indexing term with which is the actual term
	/// corresponding to position given by the user or if no position is given it is used the first admissible term as indexing term.
	/// Then filling the data structures invoking the initializeIndexMaps method.
	pair<bool, index_object> createIndex(vector<unsigned int>& bind);
};

/**
 * @brief This class is a more advanced implementation of IndexAtom (@see IndexAtom)
 * @details Searching for match is performed over a tables of facts and no facts indexed by a single term, namely the indexing term.
 * Instead of performing the search among the whole tables it is performed among the facts and the no facts, collectively the instances,
 * that match for that term.
 * Here is used as data structure an unordered multimap of pairs composed by a possible value of the indexing term
 * and an instance in which that term appears with that value.
 * This implementation is sibling to the one with maps @see SingleTermIndexAtom,
 * since the two are implemented to be independent.
 */
class SingleTermIndexAtomMultiMap: public SimpleIndexAtom {
public:
	/**
	 * Constructor
	 * @param facts An AtomTable of facts
	 * @param no facts An AtomTable of no facts
	 * @param delta An AtomTable of no facts containing the result of the previous grounding iteration
	 * @param predicate The predicate corresponding to the facts and the no facts.
	 */
	SingleTermIndexAtomMultiMap(AtomTable* facts, AtomTable* nofacts,AtomTable* delta,Predicate *p) : SimpleIndexAtom(facts,nofacts,delta,p), instantiateIndexMaps(false), positionOfIndexing(0),positionOfIndexingSetByUser(false){};
	///Overload of firstMatch method
	virtual unsigned int firstMatch(bool searchInDelta, Atom *templateAtom, map_term_term& currentAssignment, bool& find);
	//Virtual method implementation
	virtual void updateDelta(AtomTable* nextDelta);
	///Destructor
	~SingleTermIndexAtomMultiMap(){};

private:
	///Data structure for indexed facts
	unordered_multimap<index_object,GenericAtom*> factsIndexMap;
	///Data structure for indexed no facts
	unordered_multimap<index_object,GenericAtom*> nofactsIndexMap;
	///Data structure for indexed delta
	unordered_multimap<index_object,GenericAtom*> deltaIndexMap;
	///Determine whether the indexing has been filled in
	bool instantiateIndexMaps;
	///The position of the indexing term
	unsigned int positionOfIndexing;
	///Determine whether the indexing term has been established from command line arguments by the user
	bool positionOfIndexingSetByUser;

	///This method fills in the indexing data structures
	void initializeIndexMaps();

	/// This method carry out the indexing strategy, determining the indexing term with which is the actual term
	/// corresponding to position given by the user or if no position is given it is used the first admissible term as indexing term.
	/// Then filling the data structures invoking the initializeIndexMaps method.
	pair<bool, index_object> createIndex(vector<unsigned int>& bind);
};


#endif /* INSTANCE_H_ */
