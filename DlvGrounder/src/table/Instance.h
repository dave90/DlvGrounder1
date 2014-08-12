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

///This struct implements a generic atom composed by just its terms
struct GenericAtom{
	vector<index_object> terms;

	GenericAtom(){}
	GenericAtom(const vector<index_object>& t): terms(move(t)){}

	virtual bool isFact() {return true;}
	virtual void setFact(bool isFact){}

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
	vec_pair_index_object bind;

	hashAtomResult(){}

	hashAtomResult(vec_pair_index_object &bind){this->bind=bind;}

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
			terms.push_back(atom->terms[t.first]);
	}
};

///An unordered set of generic atoms by hashAtom @see hashAtom
typedef unordered_set<GenericAtom*, hashAtom, hashAtom> AtomTable;
///An unordered set of generic atoms defined by hashResultAtom @see hashResultAtom
typedef unordered_set<GenericAtom*, hashAtomResult, hashAtomResult> AtomResultTable;


///This struct implements an AtomResultTable (@see AtomResultTable) that represents a set of possible assignments for bind variables
struct ResultMatch {
	mutable AtomResultTable result;
	ResultMatch(vec_pair_index_object &bind): result(AtomResultTable(0,hashAtomResult(bind),hashAtomResult(bind))){};
};


/**
 * This class is an abstract base class that models a general indexing strategy of a predicate's instances.
 * It is builded according to the Strategy GOF pattern.
 */

class IndexAtom {
public:
	//Constructor for all the fields
	IndexAtom(AtomTable* facts, AtomTable* nofacts, Predicate *p) : facts(facts), nofacts(nofacts), predicate(p) {};
	///This method implementation is demanded to sub-classes.
	///It have to find all the matching facts and no facts and return just the first of those.
	///The returned integer will be used to get the other ones through nextMatch method @see nextMatch(unsigned int id, vec_pair_index_object &bind, bool& find)
	virtual unsigned int firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,vec_pair_index_object& boundFunction,vec_pair_index_object &bindFunction, map_int_int& equal_var, bool& find)=0;
	///This method is used to get the further matching facts and no facts one by one each time it is invoked.
	virtual void nextMatch(unsigned int id, vec_pair_index_object &bind,vec_pair_index_object& boundFunction,vec_pair_index_object &bindFunction, bool& find)=0;
	///Destructor
	virtual ~IndexAtom() {};
protected:
	//A pointer to the set of facts
	AtomTable* facts;
	//A pointer to the set of no facts, that are undefined atoms
	AtomTable* nofacts;
	///The predicate
	Predicate* predicate;
};

/**
 * @brief This class is a basic and simple implementation of IndexAtom (@see IndexAtom)
 * @details Searching for match is performed over the whole tables of facts and non facts with a linear scan.
 */
class SimpleIndexAtom: public IndexAtom {
public:
	///Constructor
	SimpleIndexAtom(AtomTable* facts, AtomTable* nofacts,Predicate *p) : IndexAtom(facts,nofacts,p), counter(0){};
	///Virtual method implementation
	virtual unsigned int firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,vec_pair_index_object& boundFunction,vec_pair_index_object &bindFunction, map_int_int& equal_var, bool& find);
	///Virtual method implementation
	virtual void nextMatch(unsigned int id, vec_pair_index_object &bind,vec_pair_index_object&boundFunction,vec_pair_index_object &bindFunction, bool& find);
	///Destructor
	virtual ~SimpleIndexAtom() {};
protected:
	///The unordered map used to store the integer identifiers returned by the firstMach method
	unordered_map<unsigned int, ResultMatch*> matches_id;
	///The counter used to assign the integer identifiers returned by the firstMach method
	unsigned int counter;

	///This method given an AtomTable computes the matching facts and nofacts and returns the first one of those
	void computeFirstMatch(AtomTable* collection,vec_pair_index_object &bound,vec_pair_index_object &bind,vec_pair_index_object& boundFunction,vec_pair_index_object &bindFunction,map_int_int& equal_var,ResultMatch* rm);
	///This method checks the matching of bind equal variable, that are variables that appear more than one times in the atom.
	bool checkEqualVariable(map_int_int& equal_var,GenericAtom *atom);
	///This method invokes findIfAFactExists method if all the variables are bound, otherwise invokes the computeFirstMatch method
	bool searchForFirstMatch(AtomTable* table, vec_pair_index_object &bound,vec_pair_index_object &bind,vec_pair_index_object& boundFunction,vec_pair_index_object &bindFunction,map_int_int& equal_var,ResultMatch* rm);
	///This method builds a ground atom using the bound variables and checks if it is true
	virtual bool findIfAFactExists(AtomTable* collection,vec_pair_index_object& bound, map_int_int& equal_var);
	/// This method check if a generic atom match with the bound function and put the value of bind variable in bindFunction
	virtual bool checkFunctionVariable(GenericAtom *genericAtom,vec_pair_index_object &boundFunction,vec_pair_index_object &bindFunction);
};

/**
 * This class represents the instances (the facts and the no facts) of a predicate
 */

class Instances {
public:
	///Constructor
	Instances(index_object predicate,PredicateTable *pt);

	bool addFact(const vector<index_object>& terms) {
		GenericAtom* atomFact=new GenericAtom(terms);
		if(!facts.insert(atomFact).second){
			delete atomFact;
			return false;
		}
		return true;
	};

	///This method looks for a generic atom in both the facts and no facts tables @see GenericAtom
	GenericAtom* getGenericAtom(vector<index_object>& terms) {
		GenericAtom* atomFact=new GenericAtom(terms);
		auto it=facts.find(atomFact);
		if(it==facts.end())
			it=nofacts.find(atomFact);

		delete atomFact;
		return *it;
	};

	///This method adds a no facts to the no facts table.
	///Its truth value can be true or undefined, if it false it is not stored at all
	bool addNoFact(GenericAtom*& atomUndef, bool truth) {
		if(!nofacts.insert(atomUndef).second){
			// If the atom is not present, it is added. The temporary atom duplicate is deleted and the inserted atom is assigned.
			GenericAtom* atomToDelete=atomUndef;
			atomUndef=*nofacts.find(atomToDelete);
			delete atomToDelete;
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

	///Printer method
	void print(){for(GenericAtom*fact:facts){ClassicalLiteral::print(predicate,fact->terms,false,false); cout<<"."<<endl;}};

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
};

/**
 * This class stores the instances for all the predicate
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
	 * @param no facts An AtomTable of no facts
	 * @param predicate The predicate corresponding to the facts and the no facts.
	 * Calling this constructor subsumes that the indexing term is not set by the user
	 */
	SingleTermIndexAtom(AtomTable* facts, AtomTable* nofacts,Predicate *p) : SimpleIndexAtom(facts,nofacts,p), instantiateIndexMaps(false), positionOfIndexing(0), positionOfIndexingSetByUser(false){};
	/**
	 * Constructor
	 * @param facts An AtomTable of facts
	 * @param no facts An AtomTable of no facts
	 * @param i The position for the indexing term
	 * @param predicate The predicate corresponding to the facts and the no facts.
	 * Calling this constructor subsumes that the indexing term is set by the user
	 */
	SingleTermIndexAtom(AtomTable* facts, AtomTable* nofacts, int i, Predicate *p): SimpleIndexAtom(facts,nofacts,p), instantiateIndexMaps(false), positionOfIndexing(i), positionOfIndexingSetByUser(true){};
	///Overload of firstMatch method
	unsigned int firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,vec_pair_index_object& boundFunction,vec_pair_index_object bindFunction, map_int_int& equal_var,bool& find);
	///Destructor
	~SingleTermIndexAtom(){};
private:
	///Data structure for indexed facts
	unordered_map<index_object,AtomTable> factsIndexMap;
	///Data structure for indexed no facts
	unordered_map<index_object,AtomTable> nofactsIndexMap;
	///Determine whether the indexing has been filled in
	bool instantiateIndexMaps;
	///The position of the indexing term
	unsigned int positionOfIndexing;
	///Determine whether the indexing term has been established from command line arguments by the user
	bool positionOfIndexingSetByUser;

	///This method fills in the indexing data structures
	void initializeIndexMaps();
	///This method determines which is the actual term corresponding to position given by the user,
	///if no position is given it is used the first admissible term as indexing term
	void determineTermToBeIndexed(vec_pair_index_object& bound);
	///This method carry out the indexing strategy, determining the indexing term with determineTermToBeIndexed method and then filling the
	///data structures with initializeIndexMaps method
	pair<bool, index_object> createIndex(vec_pair_index_object& bound);
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
	 * @param predicate The predicate corresponding to the facts and the no facts.
	 * Calling this constructor subsumes that the indexing term is not set by the user
	 */
	SingleTermIndexAtomMultiMap(AtomTable* facts, AtomTable* nofacts,Predicate *p) : SimpleIndexAtom(facts,nofacts,p), instantiateIndexMaps(false), positionOfIndexing(0),positionOfIndexingSetByUser(false){};
	/**
	 * Constructor
	 * @param facts An AtomTable of facts
	 * @param no facts An AtomTable of no facts
	 * @param i The position for the indexing term
	 * @param predicate The predicate corresponding to the facts and the no facts.
	 * Calling this constructor subsumes that the indexing term is set by the user
	 */
	SingleTermIndexAtomMultiMap(AtomTable* facts, AtomTable* nofacts, int i,Predicate *p): SimpleIndexAtom(facts,nofacts,p), instantiateIndexMaps(false), positionOfIndexing(i),positionOfIndexingSetByUser(true){};
	///Overload of firstMatch method
	unsigned int firstMatch(vec_pair_index_object &bound, vec_pair_index_object &bind,vec_pair_index_object& boundFunction,vec_pair_index_object &bindFunction, map_int_int& equal_var, bool& find);
	///Destructor
	~SingleTermIndexAtomMultiMap(){};

private:
	///Data structure for indexed facts
	unordered_multimap<index_object,GenericAtom*> factsIndexMap;
	///Data structure for indexed no facts
	unordered_multimap<index_object,GenericAtom*> nofactsIndexMap;
	///Determine whether the indexing has been filled in
	bool instantiateIndexMaps;
	///The position of the indexing term
	unsigned int positionOfIndexing;
	///Determine whether the indexing term has been established from command line arguments by the user
	bool positionOfIndexingSetByUser;

	///This method fills in the indexing data structures
	void initializeIndexMaps();
	///This method determines which is the actual term corresponding to position given by the user,
	///if no position is given it is used the first admissible term as indexing term
	void determineTermToBeIndexed(vec_pair_index_object& bound);
	///This method carry out the indexing strategy, determining the indexing term with determineTermToBeIndexed method and then filling the
	///data structures with initializeIndexMaps method
	pair<bool, index_object> createIndex(vec_pair_index_object& bound);
};


#endif /* INSTANCE_H_ */
