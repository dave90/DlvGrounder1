/*
 * AtomFactory.cpp
 *
 *  Created on: 27/mar/2014
 *      Author: Davide
 */

#include "RuleFactory.h"

#include "atom/ClassicalLiteral.h"
#include "atom/BuiltInAtom.h"

RuleFactory::RuleFactory() {
	// Initialize all variable

	head=true;
	currentRule=new Rule;
	binop=Binop::NONE_OP;

	predicateTable=new PredicateTable;
	instancesTable=new InstancesTable(predicateTable);
	statementDependency=new StatementDependency;
}


void RuleFactory::addFact(Atom *fact) {
	// Add a Instances in instaces table and then add the fact

	index_object predicate=fact->getPredicate().second;
	instancesTable->addInstance(predicate,true);
	instancesTable->getInstance(predicate)->addFact(fact->getTerms());
	delete fact;
}

void RuleFactory::addClassicalAtom(vector<index_object> &terms, bool hashMinus,
		bool negative) {
	/// Create classical literal adding the predicate in the predicate table and after add it in the rule

	Predicate *p=new Predicate(lastPredicate,terms.size());
	index_object predIndex=predicateTable->insertPredicate(p);
	Atom *a=new ClassicalLiteral(predIndex,terms,hashMinus,negative);
	if(head){
		currentRule->addInHead(a);
	}else
		currentRule->addInBody(a);
}

void  RuleFactory::setBinop(string& binop){
	// Set the binop operator given the operator string

	if(binop.compare(">")==0)
		this->binop=Binop::GREATER;
	else	if(binop.compare("<")==0)
		this->binop=Binop::LESS;
	else if(binop.compare("=")==0)
		this->binop=Binop::EQUAL;
	else if(binop.compare(">=")==0)
		this->binop=Binop::GREATER_OR_EQ;
	else if(binop.compare("<=")==0)
		this->binop=Binop::LESS_OR_EQ;
	else if(binop.compare("!=")==0 || binop.compare("<>")==0)
		this->binop=Binop::UNEQUAL;
}

void RuleFactory::addBuiltinAtom(vector<index_object> &terms) {
	Atom *a=new BuiltInAtom(binop,false,terms);
	if(head){
		currentRule->addInHead(a);
	}else
		currentRule->addInBody(a);
}

void RuleFactory::addRule() {
	// If a rule have one literal in a head is a fact and add it in InstanceTable
	// else set the predicate in the rule IDB and add the rule in the StatementDependency


	if(currentRule->isAFact()){
		Atom *fact=*currentRule->getBeginHead();

		addFact(fact);

		currentRule->clear();

	}else{
		// Set predicate in head IDB
		unordered_set<index_object> pred_head=currentRule->getPredicateInHead();
		for(index_object p:pred_head)predicateTable->setIdb(p);

		statementDependency->addRuleMapping(currentRule);

		currentRule = new Rule;
	}

	head=true;
}

