/*
 * ProgramGrounder.h
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#ifndef PROGRAMGROUNDER_H_
#define PROGRAMGROUNDER_H_

#include<vector>

#include "../table/PredicateTable.h"
#include "../table/Instance.h"
#include "../table/TermTable.h"
#include "StatementDependency.h"
#include "../statement/GroundRule.h"

using namespace std;

typedef vector<unordered_set<index_object>> vector_set_index;

///This struct implements an hash function and an equality comparator for ground rules @see GroundRule
struct hashRule {

	/// The hash of a ground rule is made combining the hash of the ground atoms in the rule
	size_t operator()(GroundRule* rule) const {
	  HashVecInt *hash=HashVecInt::getHashVecIntFromConfig();
	  vector<size_t> atomHash;
	  for(auto it=rule->getBeginHead();it!=rule->getEndHead();it++){
		  atomHash.push_back((*it)->getHash());
	  }
	  for(auto it=rule->getBeginBody();it!=rule->getEndBody();it++)
		  atomHash.push_back((*it)->getHash());
	  return hash->computeHashSize_T(atomHash);
	}

	bool operator()( GroundRule* r1,  GroundRule* r2)const{
	  return *r1==*r2;
	}

};

/**
 * 	This class contains the set of the grounded rules
 */
class GroundedRules{
public:

	GroundedRules(){}

	///This method adds a ground rule if is not already present in the set, otherwise it is deleted
	bool addRule(GroundRule* r)
	{
		if(!groundedRules.insert(r).second) {delete r;return false;};
		return true;
	}

	~GroundedRules(){for(auto rule:groundedRules)delete rule;}

private:
	///An unordered set of ground rule @see hashRule
	unordered_set<GroundRule*,hashRule,hashRule> groundedRules;
};


/**
 * @brief This class manages and executes the grounding process.
 */
class ProgramGrounder {

public:
	/** Constructor
	 *	@param pt A pointer to the predicate table
	 *	@param it A pointer to the instances table
	 *	@param st A pointer to the statement dependency table
	 *	Since the term table is a singleton, it is not needed to give it as a parameter.
	 */
	ProgramGrounder(PredicateTable* pt,InstancesTable* it,StatementDependency* st):
		predicateTable(pt), instancesTable(it), statementDependency(st), termsMap(TermTable::getInstance()){};

	///This method executes the overall grounding process
	void ground();
	/// This method executes the grounding of a single recursive rule
	/// @param r The rule to be grounded
	bool groundRule(Rule* r, bool isRecursive, bool firstIteraction);

	///Printer method for
	void print(){statementDependency->print();};

	//Printer method for facts
	void printFact(){instancesTable->print();};

	///Destructor
	virtual ~ProgramGrounder();

private:
	///A pointer to the predicate table
	PredicateTable* predicateTable;
	///A pointer to the instances table
	InstancesTable* instancesTable;
	///A pointer to the statement table
	StatementDependency* statementDependency;
	///A pointer to the terms table
	TermTable* termsMap;
	///The set of grounder rules
	GroundedRules groundedRule;

	/** This method determines the variables for each atom in the body of a rule.
	 *
	 * 	@param r The rule
	 * 	@param variables The vector that will be filled in with variables variables for each atom.
	 */
	void findBindVariablesRule(Rule *r,vector<unordered_set<index_object> >& variables);

	/// Printer method for the grounded rules
	bool printGroundRule(Rule *r,map_index_index& var_assign,bool isRecursive,bool firstIteration);

	///This method given an assignment for the variables return a partial ground atom of current_atom
	Atom* setBoundValue(Atom *current_atom,map_index_index& var_assign);

	///This method removes the assigned values to bind variables from the given assignment
	void removeBindValueInAssignment(unordered_set<index_object> bind_variables, map_index_index& var_assign);

	void updateDelta(Rule* r);

	// These are some printer methods used for debug purpose.
	void printAssignment(map_index_index& var_assign);
	void printVariables(vector<unordered_set<index_object>> variables_atoms);
};

#endif /* PROGRAMGROUNDER_H_ */
