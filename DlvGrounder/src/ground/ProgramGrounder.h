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
	ProgramGrounder(PredicateTable* pt, InstancesTable* it, StatementDependency* st) :
			predicateTable(pt), instancesTable(it), statementDependency(st), termsMap(TermTable::getInstance()){
	}
	;

	///This method executes the overall grounding process
	void ground();

	/// This method executes the grounding of a rule
	/// @param r The rule to be grounded
	/// @parm isRecursive If the rule is recursive
	/// @param firstIteraction If it is the first iteration or not (useful for recursive rules)
	bool groundRule(Rule* r, bool isRecursive, bool firstIteraction, const set_predicate* predicateInHead);

	///Printer method
	void print() {
		statementDependency->print();
	}
	;

	//Printer method for facts
	void printFact() {
		instancesTable->print();
	}
	;

	/// Return the InstanceTable
	InstancesTable* getInstanceTable() {
		return instancesTable;
	}
	;
	/// Return the StatementDependency
	StatementDependency* getStatementDependency() {
		return statementDependency;
	}
	;
	/// Return the PredicateTable
	PredicateTable* getPredicateTable() {
		return predicateTable;
	}
	;

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

	/// Current assignment for grounding rule
	/// The map of the assignment, map each variables to its assigned value
	map_term_term current_var_assign;
	/// Current id of first match for grounding rule
	list<unsigned int> current_id_match;
	/// Current variables for each atom for grounding rule
	vector<set_term> current_variables_atoms;
	/// Current rule
	Rule* currentRule;
	/// Current atom for grounding rule
	Atom *current_atom;
	/// Current atom iterator for grounding rule
	vector<Atom*>::iterator current_atom_it;
	/// Current atom index for grounding rule
	unsigned int index_current_atom;



	/// This method determines the variables for each atom in the body of a rule.
	/// @param r The rule
	///	@param variables The vector that will be filled in with variables for each atom.
	void findBindVariablesRule();

	///This method given an assignment for the variables return a partial ground atom of current_atom
	Atom* setBoundValue();

	///This method removes the assigned values to bind variables from the given assignment
	void removeBindValueInAssignment(set_term bind_variables);

	///This method update the delta table according to the last iteration (useful for recursive rules)
	void updateDelta(Rule* r);

	// Ground atom like builtIn and negative that have only buond atom
	bool groundBoundAtom( bool &find, bool negation, bool searchDelta, Instances* instance, Atom*& templateAtom);

	/// Call first match or next match for the current atom
	IndexAtom* firstNextMatch( bool searchDelta, Instances* instance, bool& firstMatch, Atom*& templateAtom, bool& find);

	///Skip the built in and negative atom for the nextMatch
	///@param firstSkip if true go back
	///@param finish change in true if the grounding of rule have to finish
	void skipAtom(bool firstSkip, bool& finish);

	// These are some printer methods used for debug purpose.
	void printAssignment();
	void printVariables();

};

#endif /* PROGRAMGROUNDER_H_ */
