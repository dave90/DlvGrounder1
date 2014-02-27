/*
 * TermFactory.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "TermFactory.h"

TermFactory::TermFactory() {

	termsMap=new MapTermTable;

}

void TermFactory::createVariable(string variable,bool negative) {
	Term *v=new VariableTerm();

	v->setVariable(variable);
	v->setNegative(negative);

	int index=termsMap->addTerm(v);

	if(terms.size()>0){
		terms[terms.size()-1]->addTerm(index);
	}
}



void TermFactory::createConstant(string constant,bool negative) {
	Term *c=new ConstantTerm();

	c->setCostant(constant);
	c->setNegative(negative);

	int index=termsMap->addTerm(c);

	if(terms.size()>0){
		terms[terms.size()-1]->addTerm(index);
	}

}

void TermFactory::createFunction(string name,bool negative) {
	Term *ft=new FunctionTerm();

	ft->setCostant(name);
	ft->setNegative(negative);

	terms.push_back(ft);
}

void TermFactory::endFunction() {
	Term *ft=terms[terms.size()-1];
	terms.pop_back();

	int index=termsMap->addTerm(ft);

	if(terms.size()>0){
		terms[terms.size()-1]->addTerm(index);
	}
}

TermFactory::~TermFactory() {
	delete termsMap;
}
