/*
 * TermFactory.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "TermFactory.h"

#include "utility/Config.h"

TermFactory::TermFactory() {
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

 long TermFactory::createVariable(string variable,bool negative) {
	Term *v=new VariableTerm();

	v->setName(variable);
	v->setNegative(negative);

	int index=termsMap->addTerm(v);

	return addTermsDependency(index);
}



 long TermFactory::createConstant(string constant,bool negative) {
	Term *c=new ConstantTerm();

	c->setName(constant);
	c->setNegative(negative);

	int index=termsMap->addTerm(c);

	return addTermsDependency(index);

}

void TermFactory::createFunction(string name,bool negative) {
	Term *ft=new FunctionTerm();

	ft->setName(name);
	ft->setNegative(negative);

	terms.push_back(ft);
}

 long TermFactory::endFunction() {
	Term *ft=terms[terms.size()-1];
	terms.pop_back();

	int index=termsMap->addTerm(ft);


	return addTermsDependency(index);

}

 long TermFactory::addTermsDependency(unsigned long index) {
	if(terms.size()>0){
		terms[terms.size()-1]->addTerm(index);


		return -1;
	}else{
		return index;
	}
}

void TermFactory::addArithTerm() {
	Term *t=new ArithTerm(termsMap);
	terms.push_back(t);
}

void TermFactory::setArithOperator(string op){
	Term* arith=terms[terms.size()-1];

	if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::PLUS).c_str())==0)
		arith->setOperator(Operator::PLUS);
	else if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::MINUS).c_str())==0)
		arith->setOperator(Operator::MINUS);
	else if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::TIMES).c_str())==0)
		arith->setOperator(Operator::TIMES);
	else if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::DIV).c_str())==0)
		arith->setOperator(Operator::DIV);

}

 long TermFactory::endArithTerm(){
	Term *at=terms[terms.size()-1];
	terms.pop_back();

	int index=termsMap->addTerm(at);


	return addTermsDependency(index);
}

