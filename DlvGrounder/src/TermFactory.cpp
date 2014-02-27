/*
 * TermFactory.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "TermFactory.h"

TermFactory::TermFactory() {


}

void TermFactory::createVariable(string variable) {
	Term *v=new VariableTerm();

	v->setVariable(variable);

	int index=termsMap.addTerm(v);

	if(terms.size()>0){
		terms[terms.size()-1]->addTerm(index);
	}
}



void TermFactory::createConstant(string constant) {
	Term *c=new ConstantTerm();

	c->setCostant(constant);

	int index=termsMap.addTerm(c);

	if(terms.size()>0){
		terms[terms.size()-1]->addTerm(index);
	}

}

void TermFactory::createFunction(string name) {
	Term *ft=new FunctionTerm();
	ft->setCostant(name);
	terms.push_back(ft);
}

void TermFactory::endFunction() {
	Term *ft=terms[terms.size()-1];
	terms.pop_back();

	int index=termsMap.addTerm(ft);

	if(terms.size()>0){
		terms[terms.size()-1]->addTerm(index);
	}
}
