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

void RuleFactory::addPredicate(string name) {
	lastPredicate=name;
}

void RuleFactory::addFact(const vector<unsigned long>& terms,
		bool hashMinus, bool negative) {
	Predicate *p=new Predicate(lastPredicate,terms.size());
	unsigned long index=predicateTable->insertPredicate(p);
	instancesTable->addInstance(index);
	Atom* atom=new ClassicalLiteral(index,terms,hashMinus,negative);
	instancesTable->getInstance(index)->addFact(atom);

}

void RuleFactory::addClassicalAtom(vector<unsigned long> &terms, bool hashMinus,
		bool negative) {
	Predicate *p=new Predicate(lastPredicate,terms.size());
	unsigned long predIndex=predicateTable->insertPredicate(p);
	Atom *a=new ClassicalLiteral(predIndex,terms,hashMinus,negative);
	if(head){
		currentRule->addInHead(a);
	}else
		currentRule->addInBody(a);
}

void  RuleFactory::setBinop(string binop){
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

void RuleFactory::addBuiltinAtom(vector<unsigned long> &terms,TermTable*tt) {
	Atom *a=new BuiltInAtom(tt,terms[0],terms[1],binop,false);
	if(head){
		currentRule->addInHead(a);
	}else
		currentRule->addInBody(a);
}

void RuleFactory::addRule() {
	if(currentRule->isAFact()){
		Atom *fact=currentRule->getHead()[0];
		addFact(fact->getTerms(),fact->isHasMinus(),fact->isNegative());
	}else{
		// Set predicate in head IDB
		unordered_set<unsigned long> pred_head=currentRule->getPredicateInHead();
		for(unsigned long p:pred_head)predicateTable->setIdb(p);

		st->addRuleMapping(currentRule);
	}

	head=true;
	currentRule=new Rule;
}

