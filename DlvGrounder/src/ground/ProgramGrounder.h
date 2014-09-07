/*
 * ProgramGrounder.h
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#ifndef PROGRAMGROUNDER_H_
#define PROGRAMGROUNDER_H_

#include <vector>

#include "ProgramEvaluator.h"
#include "../table/PredicateTable.h"
#include "../table/Instance.h"
#include "../table/TermTable.h"
#include "StatementDependency.h"
#include "../statement/GroundRule.h"

using namespace std;

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

	/// Return the InstanceTable
	InstancesTable* getInstanceTable(){return instancesTable;};
	/// Return the StatementDependency
	StatementDependency* getStatementDependency(){return statementDependency;};
	/// Return the PredicateTable
	PredicateTable* getPredicateTable(){return predicateTable;};

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
	/// Manage the output and simplification
	ProgramEvaluator evaluator;

	/// This method determines the variables for each atom in the body of a rule.
	/// @param r The rule
	///	@param variables The vector that will be filled in with variables for each atom.
	void findBindVariablesRule(Rule *r,vector<unordered_set<index_object> >& variables);

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
