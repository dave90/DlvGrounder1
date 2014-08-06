/*
 * StatementBuilder.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef STATEMENTBUILDER_H_
#define STATEMENTBUILDER_H_


#include <string>

#include <boost/timer.hpp>

#include "TermFactory.h"
#include "RuleFactory.h"

using namespace std;

/**
 *  StatementBuilder is a statement builder, each time a parser find a statement
 *  a relative function of StatementBuilder is called
 */
class StatementBuilder {
public:
	StatementBuilder();

	/// Print the statistics
	void printStats();
	/// Add statement
	void addStatement(){statement++;};
	/// Add weak constraint
	void addWeak(){weak++;};
	/// Add choiche
	void addChoiche(){choice++;};
	/// Add aggregate
	void addAggregate(){aggregate++;};
	/// Find ':-' in a rule, then all the litteral encountered
	/// henceforth are in the body
	void setRuleBody(){ruleFactory.setAtomInHead(false);};
	/// Add rule
	void addRule (){ruleFactory.addRule();};
	/// Add predicate
	void addLiteral(string& name){
		ruleFactory.addPredicate(name);
		literal++;
	};
	/// Add classical literal
	void addClassicalAtom();
	/// Set binop for builtin atom
	void setBinop(string& binop){ruleFactory.setBinop(binop);};
	/// Add builtin atom
	void addBuiltinAtom();
	/// Set the current Atom a negative atom
	void setNegativeAtom(){	negativeAtom = true;};
	// Set the current Atom a strong negative atom
	void setStrongNegativeAtom(){	hashMinusAtom = true;};
	/// Reset all variable used for parser a term
	void resetTerm(){
		id = "";
		negativeTerm = false;
	}
	/// Add variable term
	void addVariable(string & name);
	/// Add constant term
	void addId(string & name);
	/// Add numeric costant term
	void addNumber(unsigned int & name);
	/// Set the name of the function
	void addNameFunction(string & name){id = name;}
	/// Add a function term in the queue of the parser function
	void addTermFunction();
	/// Add a functional term
	void endTermFunction(){
		termsFactory.endFunction();
		resetTerm();
	};
	/// Add arithmetic term
	void addArithTerm(){
		termsFactory.addArithTerm();
		resetTerm();
	};
	/// Set the current term a negative term
	void setNegativeTerm(){	negativeTerm = true;}
	/// Set operator of ArithTerm
	void setArithOperator(string& op){	termsFactory.setArithOperator(op);};
	/// End arith term
	void endArithTerm(){termsFactory.endArithTerm();}
	/// Remove last term
	void removeLastTerm(){termsFactory.removeLastTerm();};

	/// Return the InstanceTable
	InstancesTable* getInstanceTable(){return ruleFactory.getInstancesTable();};
	/// Return the StatementDependency
	StatementDependency* getStatementDependency(){return ruleFactory.getStatementDependency();};
	/// Return the PredicateTable
	PredicateTable* getPredicateTable(){return ruleFactory.getPredicateTable();};

private:
	/*
	 *  Counter for the statistics
	 */
	int statement;
	int weak;
	int constraint;
	int choice;
	int aggregate;
	int literal;
	int term;

	/*
	 * Variable for parser a term
	 */
	bool negativeTerm;
	string id;

	/*
	 *  Variable for parsing atoms
	 */
	bool hashMinusAtom;
	bool negativeAtom;

	/**
	 *  Factory of terms
	 */
	TermFactory termsFactory;

	/**
	 *  Factory of rules
	 */
	RuleFactory ruleFactory;
};

#endif /* STATEMENTBUILDER_H_ */
