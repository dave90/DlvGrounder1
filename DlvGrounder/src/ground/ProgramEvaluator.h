/*
 * ProgramEvaluator.h
 *
 *  Created on: Sep 7, 2014
 *      Author: david
 */

#ifndef PROGRAMEVALUATOR_H_
#define PROGRAMEVALUATOR_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "../table/PredicateTable.h"
#include "../table/Instance.h"
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

	inline bool operator()( GroundRule* r1,  GroundRule* r2)const{
	  return *r1==*r2;
	}

	/// The hash of a ground atom
	inline size_t operator()(GroundAtom* atom) const {
	  return atom->predicate->getIndex()+HashVecInt::getHashVecIntFromConfig()->computeHashTerm(atom->atom->terms);
	}

	inline bool operator()( GroundAtom* a1,  GroundAtom* a2)const{
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
		return true;
	}

	/// Return the first rule grounded
	vector<GroundRule*>::const_iterator getBeginGroundOrder(){return groundedRulesOrdering.begin();};

	/// Return the last rule grounded
	vector<GroundRule*>::const_iterator getEndGroundOrder(){return groundedRulesOrdering.end();};


	~GroundedRules(){for(auto rule:groundedRules)delete rule;}

private:
	///An unordered set of ground rule @see hashRule
	unordered_set<GroundRule*,hashRule,hashRule> groundedRules;
	///Order of the rule when grounding
	vector<GroundRule*> groundedRulesOrdering;

};

/**
 *  This class manage the ouput and the simplification of the program
 */
class ProgramEvaluator {
public:
	ProgramEvaluator(){simplification=!Config::getInstance()->isDisableSimplification();};

	bool addRule(GroundRule *r){
		if(!rules.addRule(r))return false;
		incrementSupport(r);
		return true;
	}

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

	///Increment the support of all the atom in the head of rule
	void incrementSupport(GroundRule* rule){
		for(auto head_it=rule->getBeginHead();head_it!=rule->getEndHead();head_it++)incrementSupport(*head_it);
	}

	/// Print the rule and simplify if is possible
	void printAndSimplify(InstanceTable* instancesTable);

	/// Printer method for the grounded rules according to the given assignment
	bool printGroundRule(InstanceTable* instancesTable,PredicateTable * predicateTable,StatementDependency * statementDep,Rule *r, map_term_term& var_assign, bool isRecursive, bool firstIteration);

	virtual ~ProgramEvaluator(){};
private:
	/// Number of supported rule for each atom
	unordered_map<GroundAtom*,unsigned int,hashRule,hashRule> supportedAtom;

	GroundedRules rules;

	bool simplification;

	/// Ground the body of the rule
	bool groundBody(bool disjunction, bool isRecursive, bool firstIteration,
			bool updated, Rule* r, map_term_term& var_assign,
			InstanceTable* instancesTable, GroundRule* groundRule,
			bool& added);

	/// Ground the head of the rule
	void groundHead(Rule* r, PredicateTable* predicateTable,
			map_term_term& var_assign, InstanceTable* instancesTable,
			StatementDependency* statementDep, GroundRule* groundRule);

	///Ground the constraint with no simplification
	void groundConstraint(Rule* r, PredicateTable* predicateTable, map_term_term& var_assign);
};

#endif /* PROGRAMEVALUATOR_H_ */
