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

typedef unordered_map<index_object,index_object> map_index_index;

///This struct implements a generic atom composed by just its terms
struct GenericAtom{
	vector<index_object> terms;

	GenericAtom(){}
	GenericAtom(const vector<index_object>& t): terms(move(t)){}

	virtual bool isFact() {return true;}
	virtual void setFact(bool isFact){};

	inline bool operator==(const GenericAtom& genericAtom) const{
		if(terms.size()!=genericAtom.terms.size())
			return false;
		return equal(terms.begin(),terms.end(),genericAtom.terms.begin());
	}

	virtual ~GenericAtom(){}
};

///This struct implements an undefined atom extending GenericAtom (@see GenericAtom) adding a boolean value to determine whether it is true or not.
///An undefined atoms is an atom derived during the grounding process.
struct AtomUndef : GenericAtom{
	bool fact;

	AtomUndef(vector<index_object>& t,bool f): GenericAtom(t), fact(f) {}

	bool isFact(){return fact;}
	void setFact(bool fact){this->fact=fact;};
};

///A vector of pair of unsigned integer values and index_object values @see IndexDefinition
typedef vector<pair<unsigned int,index_object>> vec_pair_index_object;
///An unordered map with unsigned integers as keys and values
typedef unordered_multimap<unsigned int,unsigned int> map_int_int;

///This struct implements an hash function and an equality comparator for generic atoms @see GenericAtom
struct hashAtom {
	size_t operator()(GenericAtom* atomFact) const {
		return HashVecInt::getHashVecIntFromConfig()->computeHash(atomFact->terms);
	}

	bool operator()(GenericAtom* a1, GenericAtom* a2) const {
		return *a1==*a2;
	}
};


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
		vector<index_object> terms;
		getTermsBind(atom,terms);
		return HashVecInt::getHashVecIntFromConfig()->computeHash(terms);
	}

	///Equality comparison of two generic atoms over their bind variables
	bool operator()(GenericAtom* atom1,  GenericAtom* atom2) const {
		vector<index_object> terms1;
		getTermsBind(atom1,terms1);
		vector<index_object> terms2;
		getTermsBind(atom2,terms2);
		return equal(terms1.begin(),terms1.end(),terms2.begin());
	}

	///This method filters the bind variables of a generic atoms and puts them in the vector given as parameters
	void getTermsBind( GenericAtom* atom, vector<index_object>& terms) const{
		for(auto t:bind)
			terms.push_back(atom->terms[t]);
	}
};

///An unordered set of generic atoms by hashAtom @see hashAtom
typedef unordered_set<GenericAtom*, hashAtom, hashAtom> AtomTable;
///An unordered set of generic atoms defined by hashResultAtom @see hashResultAtom
typedef unordered_set<GenericAtom*, hashAtomResult, hashAtomResult> AtomResultTable;


///This struct implements an AtomResultTable (@see AtomResultTable) that represents a set of possible assignments for bind variables
struct ResultMatch {
	mutable AtomResultTable result;
	ResultMatch(vector<unsigned int> &bind): result(AtomResultTable(0,hashAtomResult(bind),hashAtomResult(bind))){};
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
	virtual unsigned int firstMatch(bool searchInDelta, Atom *templateAtom, map_index_index& currentAssignment, bool& find)=0;
	///This method implementation is demanded to sub-classes.
	///It is used to get the further matching atoms one by one each time it is invoked.
	virtual void nextMatch(unsigned int id,Atom *templateAtom, map_index_index& currentAssignment, bool& find)=0;
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

	static const int FACTS=0;
	static const int NOFACTS=1;
	static const int DELTA=2;

protected:
	//A pointer to the set of facts
	AtomTable* facts;
	//A pointer to the set of no facts, that are undefined atoms
	AtomTable* nofacts;
	//A pointer to the set of no facts derived in the previous iteration, that are undefined atoms
	AtomTable* delta;
	///The predicate
	Predicate* predicate;

	/// Return true if the genericAtom match with the template atom and insert in assignment the value of variable
	/// Two atom match if a constant term are equal.
	/// @param varAssignment map of assignment of the variable. If templateAtom have variable term put in
	/// varAssignment the ID of variable with the relative constant term of the genericAtom
	virtual bool match(GenericAtom *genericAtom,Atom *templateAtom,map_index_index& currentAssignment);
	/// Return true if the genericAtom match with the template atom
	/// Two atom match if a constant term are equal.
	virtual bool match(GenericAtom *genericAtom,Atom *templateAtom);
};

/**
 * This class represents the instances (the facts and the no facts) of a predicate
 */

class Instances {
public:
	///Constructor
	Instances(index_object predicate,PredicateTable *pt): predicate(predicate),predicateTable(pt),indexAtom(0) {this->configureIndexAtom();}

	bool addFact(const vector<index_object>& terms) {
		GenericAtom* atomFact=new GenericAtom(terms);

		bool inserted=false;
		if(facts.insert(atomFact).second)
			inserted=true;

		// If the atom is not present, it is added. The temporary atom duplicate is deleted and the inserted atom is assigned.
		indexAtom->find(IndexAtom::NOFACTS,atomFact);

		if(inserted)
			return true;
		return false;
	};

	///This method looks for a generic atom in both the facts and no facts tables @see GenericAtom
	GenericAtom* getGenericAtom(vector<index_object>& terms) {
		GenericAtom* atomFact=new GenericAtom(terms);

		bool isFact=indexAtom->count(IndexAtom::FACTS,atomFact);
		if(isFact){
			indexAtom->find(IndexAtom::FACTS,atomFact);
			return atomFact;
		}

		bool isNoFact=indexAtom->count(IndexAtom::NOFACTS,atomFact);
		if(isNoFact){
			indexAtom->find(IndexAtom::NOFACTS,atomFact);
			return atomFact;
		}

		bool isDelta=indexAtom->count(IndexAtom::DELTA,atomFact);
		if(isDelta){
			indexAtom->find(IndexAtom::DELTA,atomFact);
			return atomFact;
		}

	return 0;
	};

	///This method adds a no facts to the no facts table.
	///Its truth value can be true or undefined, if it false it is not stored at all
	bool addNoFact(GenericAtom*& atomUndef) {
		bool isFact=indexAtom->count(IndexAtom::FACTS,atomUndef);
		if(isFact){
			// If the atom is a facts, it is returned. The temporary atom duplicate is deleted within the find method.
			indexAtom->find(IndexAtom::FACTS,atomUndef);
			return false;
		}
		// If the atom is not present, it is added. The temporary atom duplicate is deleted and the inserted atom is assigned.
		if(!nofacts.insert(atomUndef).second){
			bool updated=false;
			//If the atom in the table is undef and the atom to be insert is true then change only the value
			if(atomUndef->isFact() && !isTrue(atomUndef->terms)){
				setValue(atomUndef->terms,true);
				updated=true;
			}
			indexAtom->find(IndexAtom::NOFACTS,atomUndef);
			if(updated)
				return true;
			return false;
		}
		return true;
	};

	///This method sets a no facts truth value
	void setValue(vector<index_object>& terms, bool truth) {
		GenericAtom *atomUndef=new AtomUndef(terms,truth);
		auto it=nofacts.find( atomUndef);
		(*it)->setFact(truth);
		delete atomUndef;
	};

	///This method determines whether a no fact is true
	bool isTrue(vector<index_object>& terms) {
		GenericAtom *atomUndef=new AtomUndef(terms,0);
		auto it=nofacts.find( atomUndef);
		bool result = (*it)->isFact();
		delete atomUndef;
		return result;
	};

	///Getter for the predicate
	index_object getPredicate() const {return predicate;}
	///Setter for the predicate
	void setPredicate(index_object predicate) {this->predicate = predicate;};

	///Getter for the IndexAtom
	IndexAtom* getIndex() {return indexAtom;};

	///This method adds a no facts to the delta table if it is yet not present in the facts, no facts and delta tables.
	///Its truth value can be true or undefined, if it false it is not stored at all.
	bool addDelta(GenericAtom*& atomUndef);

	///This method adds a no facts to the next delta table if it is yet not present in the facts, no facts, delta and next delta tables.
	///Its truth value can be true or undefined, if it false it is not stored at all.
	bool addNextDelta(GenericAtom*& atomUndef);

	///This method moves the content of the delta table in the no facts table,
	///and then moves the content of the next delta table in the delta table.
	void moveNextDeltaInDelta();

	///Printer method
	void print(){for(GenericAtom*fact:facts){ClassicalLiteral::print(predicate,fact->terms,false,false); cout<<"."<<endl;}}

	///Destructor
	~Instances();

private:
	///The predicate
	index_object predicate;
	///A pointer to the predicate table
	PredicateTable* predicateTable;
	///The IndexAtom to set the indexing strategy
	IndexAtom* indexAtom;
	///The set of facts
	AtomTable facts;
	//The set of no facts, that are undefined atoms
	AtomTable nofacts;
	/// The set of no facts, computed in the previous iteration
	AtomTable delta;
	/// The set of no facts, computed in the current iteration
	AtomTable nextDelta;

	///This method configures the indexing strategy
	void configureIndexAtom();

};

/**
 * This class stores the instances for all the predicate.
 */

class InstancesTable {
public:
	///Constructor
	InstancesTable(PredicateTable *pt):predicateTable(pt){}

	///This method adds an Instance for a predicate
	void addInstance(index_object i) {
		if(!instanceTable.count(i)){
			Instances* is = new Instances(i,predicateTable);
			instanceTable.insert({i,is});
		}
	};

	///Getter for the instances of a predicate
	Instances* getInstance(index_object i) {
		auto result= instanceTable.find(i);
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
	///A pointer to the predicate table
	PredicateTable* predicateTable;
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
	virtual unsigned int firstMatch(bool searchInDelta, Atom *templateAtom, map_index_index& currentAssignment, bool& find);
	///Virtual method implementation
	virtual void nextMatch(unsigned int id,Atom *templateAtom,map_index_index& currentAssignment, bool& find);
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
	map_index_index* currentAssignment;

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
	virtual unsigned int firstMatch(bool searchInDelta, Atom *templateAtom, map_index_index& currentAssignment, bool& find);
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
	virtual unsigned int firstMatch(bool searchInDelta, Atom *templateAtom, map_index_index& currentAssignment, bool& find);
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
