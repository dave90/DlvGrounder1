/*
 * AtomFactory.cpp
 *
 *  Created on: 27/mar/2014
 *      Author: Davide
 */

#include "RuleFactory.h"

#include "atom/ClassicalLiteral.h"

RuleFactory::RuleFactory() {
	head=true;
	currentRule=new Rule;
}

void RuleFactory::addPredicate(string name) {
	lastPredicate=name;
}

void RuleFactory::addFact(const vector<unsigned long>& terms,
		bool hashMinus, bool negative) {
	Predicate p(lastPredicate,terms.size());
	unsigned long index=predicateTable.insertPredicate(p);
	instancesTable.addInstance(index);
	Atom* atom=new ClassicalLiteral(index,terms,hashMinus,negative);
	instancesTable.getInstance(index)->addFact(atom);

}

void RuleFactory::addClassicalAtom(vector<unsigned long> &terms, bool hashMinus,
		bool negative) {
	Predicate p(lastPredicate,terms.size());
	unsigned long predIndex=predicateTable.insertPredicate(p);
	Atom *a=new ClassicalLiteral(predIndex,terms,hashMinus,negative);
	if(head)
		currentRule->addInHead(a);
	else
		currentRule->addInBody(a);
}

void RuleFactory::addRule() {
	if(currentRule->isAFact()){
		Atom *fact=currentRule->getHead()[0];
		addFact(fact->getTerms(),fact->isHasMinus(),fact->isNegative());
	}else{
		rules.push_back(currentRule);
		st.addRuleMapping(currentRule);
	}

	head=true;
	currentRule=new Rule;
}

void RuleFactory::print() {
	//FIXME
	st.createDependency(rules);

	cout<<"Instances Table"<<endl;
	instancesTable.print();
	cout<<endl<<endl;
	cout<<"Rules:"<<endl;
	for(Rule *r:rules)
		r->print();
	cout<<endl<<endl;
	st.printDepGraph();
	st.printCompGraph();
}

RuleFactory::~RuleFactory() {
}
