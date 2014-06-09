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
	head=true;
	currentRule=new Rule;
	binop=Binop::NONE_OP;

	predicateTable=new PredicateTable;
	instancesTable=new InstancesTable;
	st=new StatementDependency;
}

void RuleFactory::addPredicate(string& name) {
	lastPredicate=name;
}

void RuleFactory::addFact(Atom *fact) {

	instancesTable->addInstance(fact->getPredicate());
	instancesTable->getInstance(fact->getPredicate())->addFact(fact);
}

void RuleFactory::addClassicalAtom(vector<index_object> &terms, bool hashMinus,
		bool negative) {
	Predicate *p=new Predicate(lastPredicate,terms.size());
	index_object predIndex=predicateTable->insertPredicate(p);
	Atom *a=new ClassicalLiteral(predIndex,terms,hashMinus,negative);
	if(head){
		currentRule->addInHead(a);
	}else
		currentRule->addInBody(a);
}

void  RuleFactory::setBinop(string& binop){
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

void RuleFactory::addBuiltinAtom(vector<index_object> &terms,TermTable*tt) {
	Atom *a=new BuiltInAtom(tt,terms[0],terms[1],binop,false);
	if(head){
		currentRule->addInHead(a);
	}else
		currentRule->addInBody(a);
}

void RuleFactory::addRule() {
	if(currentRule->isAFact()){
		Atom *fact=currentRule->getHead()[0];

		addFact(fact);

		currentRule->clear();

	}else{
		// Set predicate in head IDB
		unordered_set<index_object> pred_head=currentRule->getPredicateInHead();
		for(index_object p:pred_head)predicateTable->setIdb(p);

		st->addRuleMapping(currentRule);

		currentRule = new Rule;
	}

	head=true;
}

