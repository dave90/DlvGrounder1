/*
 * Instance.h
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <iostream>

#include "IndexAtom.h"

using namespace std;

///A vector of pair of unsigned integer values and index_object values @see IndexDefinition
typedef vector<pair<unsigned int,index_object>> vec_pair_index_object;
///An unordered map with unsigned integers as keys and values
typedef unordered_multimap<unsigned int,unsigned int> map_int_int;

/**
 * This class represents the instances (the facts and the no facts) of a predicate
 */

class Instance {
public:
	///Constructor
	Instance(Predicate* predicate): predicate(predicate),indexAtom(0) {
		for(unsigned int i=0;i<4;i++)
			tables.push_back(AtomTable());
		this->setIndexAtom();
	}

	///Getter for the predicate
	Predicate* getPredicate() const {return predicate;}

	///Getter for the IndexAtom
	IndexAtom* getIndex() {return indexAtom;}

	GenericAtom* getGenericAtom(unsigned int table, vector<Term*>& terms,bool truth);
	GenericAtom* getGenericAtom(vector<Term*>& terms,bool truth);

	///This method updates a no facts truth value
	void setTruth(vector<Term*>& terms, bool truth);

	///This method determines whether a no fact is true
	bool isTrue(vector<Term*>& terms);

	bool add(unsigned int table, GenericAtom*& atomUndef, bool& updated);

	///This method moves the content of the delta table in the no facts table,
	///and then moves the content of the next delta table in the delta table.
	void moveNextDeltaInDelta();

	bool findIn(unsigned int table, GenericAtom*& atomUndef, bool& updated);

	///Printer method
	inline void print(){for(GenericAtom*fact:tables[Instance::FACTS]){ClassicalLiteral::print(predicate,fact->terms,false,false); cout<<"."<<endl;}}

	///Destructor
	~Instance();

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

class InstanceTable {
public:
	///Constructor
	InstanceTable(){}

	///This method adds an Instance for a predicate
	void addInstance(Predicate* p) {
		if(!instanceTable.count(p->getIndex())){
			Instance* is = new Instance(p);
			instanceTable.insert({p->getIndex(),is});
		}
	};

	///Getter for the instances of a predicate
	Instance* getInstance(Predicate* p) {
		auto result= instanceTable.find(p->getIndex());
		if(result==instanceTable.end()) return nullptr;
		return result->second;
	};

	///Getter for the instances of a predicate index
	Instance* getInstanceIndex(index_object p) {
		auto result= instanceTable.find(p);
		if(result==instanceTable.end()) return nullptr;
		return result->second;
	};

	///This method return the size of the map of instances
	unsigned int getSize() {return instanceTable.size();};

	///Printer method
	void print() {for (auto i : instanceTable)i.second->print();	};

	///Destructor
	~InstanceTable();
private:

	///The map that stores all the Instances
	unordered_map<index_object,Instance*> instanceTable;
};

#endif /* INSTANCE_H_ */
