/*
 * TermFactory.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "TermFactory.h"

#include "utility/Config.h"

TermFactory::TermFactory() {
	arith=false;

	setTableType();
}



void TermFactory::setTableType() {
	if(Config::getInstance()->getTermTableType()==TermTableType::BOOST)
		termsMap=new BoostTermTable;
	else if(Config::getInstance()->getTermTableType()==TermTableType::STL)
		termsMap=new HashTermTable;
	else
		termsMap=new HashTermTable;
}

unsigned long TermFactory::createVariable(string variable,bool negative) {
	Term *v=new VariableTerm();

	v->setName(variable);
	v->setNegative(negative);

	int index=termsMap->addTerm(v);

	lastTerm=index;


	return addTermsDependency(index);
}



unsigned long TermFactory::createConstant(string constant,bool negative) {
	Term *c=new ConstantTerm();

	c->setName(constant);
	c->setNegative(negative);

	int index=termsMap->addTerm(c);

	lastTerm=index;

	return addTermsDependency(index);

}

void TermFactory::createFunction(string name,bool negative) {
	Term *ft=new FunctionTerm();

	ft->setName(name);
	ft->setNegative(negative);

	terms.push_back(ft);
}

unsigned long TermFactory::endFunction() {
	Term *ft=terms[terms.size()-1];
	terms.pop_back();

	int index=termsMap->addTerm(ft);


	lastTerm=index;

	return addTermsDependency(index);

}

unsigned long TermFactory::addTermsDependency(unsigned long index) {
	if(terms.size()>0){
		terms[terms.size()-1]->addTerm(index);

		if(arith){
			arith=false;
			Term *at=terms[terms.size()-1];
			terms.pop_back();
			index=termsMap->addTerm(at);
			lastTerm=index;
			if(terms.empty())return index;
		}
		return -1;
	}else{
		return index;
	}
}

unsigned long TermFactory::addArithTerm(string op) {
	Term *t=0;

	if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::PLUS).c_str())==0)
		t=new ArithTermP(termsMap);
	else if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::MINUS).c_str())==0)
		t=new ArithTermM(termsMap);
	else if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::TIMES).c_str())==0)
		t=new ArithTermT(termsMap);
	else if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::DIV).c_str())==0)
		t=new ArithTermD(termsMap);


	t->addTerm(lastTerm);
	arith=true;
	terms.push_back(t);

	if(terms.size()>1)return -1;
	return 0;
}

TermFactory::~TermFactory() {
	delete termsMap;
}


