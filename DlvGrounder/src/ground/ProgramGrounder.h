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

///A unordered map whose keys and values are of index_object type @see IndexDefinition
typedef unordered_map<index_object, index_object> map_index_index;

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
	/// This method executes the grounding of a single exit rule
	/// @param r The rule to be grounded
	void groundRule(Rule *r);
	/// This method executes the grounding of a single recursive rule
	/// @param r The rule to be grounded
	bool groundRecursiveRule(Rule* r);

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

	/** This method determines the bound and bind variables for each atom in the body of a rule.
	 * It also checks for each atom the presence of same variables more time.
	 * During grounding process the variables of an atom are classified as bind or bound.
	 * Bind variables are the ones that have no assignment yet, while the bound ones have an assignment.
	 *
	 * 	@param r The rule
	 * 	@param bounds The vector that will be filled in with bound variables for each atom.
	 * 	The ith position in this vector contains the bound variables for the ith atom in the body of the rule.
	 * 	@param binds The vector that will be filled in with bind variables for each atom.
	 * 	The ith position in this vector contains the bind variables for the ith atom in the body of the rule.
	 * 	@param equal_vars The vector that will be filled in with a map for each atom.
	 * 	The ith position in this vector contains the map of the the equal variables for the ith atom in the body of the rule.
	 */
	void findBoundBindRule(Rule *r,vector<vec_pair_index_object> &bounds,vector<vec_pair_index_object>& binds,vector<vec_pair_index_object> &boundsFunction,vector<vec_pair_index_object>& bindsFunction,vector<map_int_int >& equal_vars);

	/// Printer method for the grounded rules
	void printGroundRule(Rule *r,map_index_index& var_assign);

	///This method given an assignment for the variables, updates the assigned values for the bound variables.
	void setBoundValue(Atom *current_atom,vec_pair_index_object &bound,vec_pair_index_object &boundFunction,map_index_index& var_assign);

	///This method removes the assigned values to bind variables from the given assignment
	void removeBindValueInAssignment(Atom *current_atom,vec_pair_index_object &bind,vec_pair_index_object &bindFunction,map_index_index& var_assign);
	///This method inserts the assigned values to bind variables from the given assignment
	void insertBindValueInAssignment(Atom *current_atom,vec_pair_index_object &bind,vec_pair_index_object &bindFunction,map_index_index& var_assign);

	// These are some printer methods used for debug purpose.
	void printVecPair(string name,vector<vec_pair_index_object> &vec);
	void printPair(int i, vector<vec_pair_index_object>& vec);
	void printMapIntInt(string name,vector<map_int_int >& equal_vars);
	void printAssignment(map_index_index& var_assign);
	IndexAtom* match(const vector<vec_pair_index_object>& bounds,
			unsigned int index_current_atom,
			const vector<vec_pair_index_object>& binds,
			const vector<map_int_int>& equal_vars, bool negation,
			Instances* instance, Atom* current_atom,
			map_index_index& var_assign, bool& firstMatch,
			list<unsigned int>& id_match, bool& find);
};

#endif /* PROGRAMGROUNDER_H_ */
