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

	/// The hash of a ground atom
	size_t operator()(GroundAtom* atom) const {
	  return atom->predicate+HashVecInt::getHashVecIntFromConfig()->computeHash(atom->atom->terms);
	}

	bool operator()( GroundAtom* a1,  GroundAtom* a2)const{
		if(a1->predicate != a2->predicate)return false;
		return *a1->atom==*a2->atom;
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
		groundedRulesOrdering.push_back(r);
		for(auto head_it=r->getBeginHead();head_it!=r->getEndHead();head_it++)incrementSupport(*head_it);
		return true;
	}

	//Print the rule and simplify if is possible
	void printAndSimplify(InstancesTable* instancesTable);

	///Increment the support of atom
	void incrementSupport(GroundAtom* atom){
		if(!supportedAtom.count(atom))
			supportedAtom.insert({atom,1});
		else{
			unsigned int support=supportedAtom.find(atom)->second;
			supportedAtom.find(atom)->second=support+1;
		}
	}

	///Decrement the support of atom
	void decrementSupport(GroundAtom* atom){
		if(!supportedAtom.count(atom))
			supportedAtom.insert({atom,0});
		else{
			unsigned int support=supportedAtom.find(atom)->second;
			if(support>0)
				supportedAtom.find(atom)->second=support-1;
		}
	}

	///Decrement the support of all the atom in the head of rule
	void decrementSupport(GroundRule* rule){
		for(auto head_it=rule->getBeginHead();head_it!=rule->getEndHead();head_it++)decrementSupport(*head_it);
	}

	~GroundedRules(){for(auto rule:groundedRules)delete rule;}

private:
	///An unordered set of ground rule @see hashRule
	unordered_set<GroundRule*,hashRule,hashRule> groundedRules;
	///Order of the rule when grounding
	vector<GroundRule*> groundedRulesOrdering;
	/// Number of supported rule for each atom
	unordered_map<GroundAtom*,unsigned int,hashRule,hashRule> supportedAtom;
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

	/// This method executes the grounding of a rule
	/// @param r The rule to be grounded
	/// @parm isRecursive If the rule is recursive
	/// @param firstIteraction If it is the first iteration or not (useful for recursive rules)
	bool groundRule(Rule* r, bool isRecursive, bool firstIteraction,const unordered_set<index_object>* predicateInHead);

	///Printer method
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

	/// This method determines the variables for each atom in the body of a rule.
	/// @param r The rule
	///	@param variables The vector that will be filled in with variables for each atom.
	void findBindVariablesRule(Rule *r,vector<unordered_set<index_object> >& variables);

	/// Printer method for the grounded rules according to the given assignment
	bool printGroundRule(Rule *r,map_index_index& var_assign,bool isRecursive,bool firstIteration);

	///This method given an assignment for the variables return a partial ground atom of current_atom
	Atom* setBoundValue(Atom *current_atom,map_index_index& var_assign);

	///This method removes the assigned values to bind variables from the given assignment
	void removeBindValueInAssignment(unordered_set<index_object> bind_variables, map_index_index& var_assign);

	///This method update the delta table according to the last iteration (useful for recursive rules)
	void updateDelta(Rule* r);

	///Skip the built in and negative atom for the nextMatch
	///@param firstSkip if true go back
	///@param current_atom_it the current iterator in the rule
	///@param index_current_atom the index of the current atom in the rule
	///@param id_match the list of the id of first match
	///@param r is the rule
	///@param finish change in true if the grounding of rule have to finish
	void skipAtom(bool firstSkip,vector<Atom*>::const_iterator& current_atom_it,unsigned int& index_current_atom,list<unsigned int>& id_match,Rule *r,bool& finish);

	// These are some printer methods used for debug purpose.
	void printAssignment(map_index_index& var_assign);
	void printVariables(vector<unordered_set<index_object>> variables_atoms);
};

#endif /* PROGRAMGROUNDER_H_ */
