/*
 * TermFactory.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "TermFactory.h"

#include "utility/Config.h"




 void TermFactory::createVariable(string& variable,bool negative) {
	Term *v=new VariableTerm(variable,negative);

	termsMap->addTerm(v);

	addTermsDependency(v);
}



 void TermFactory::createConstant(string& constant,bool negative) {
	Term *c=new StringConstantTerm(constant,negative);

	termsMap->addTerm(c);

	addTermsDependency(c);

}

 void TermFactory::createConstant(unsigned& constant,bool negative) {
	Term *c=new NumericConstantTerm(constant,negative);

	termsMap->addTerm(c);

	addTermsDependency(c);

}


 void TermFactory::endFunction() {
	Term *ft=terms[terms.size()-1];
	terms.pop_back();

	termsMap->addTerm(ft);


	addTermsDependency(ft);

}

 void TermFactory::addTermsDependency(Term* index) {
	if(terms.size()>0)
		terms[terms.size()-1]->addTerm(index);
	else
		termsInAtom.push_back(index);
}

void TermFactory::setArithOperator(string& op){
	Term *arith=terms[terms.size()-1];
	if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::PLUS).c_str())==0)
		arith->setOperator(Operator::PLUS);
	else if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::MINUS).c_str())==0)
		arith->setOperator(Operator::MINUS);
	else if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::TIMES).c_str())==0)
		arith->setOperator(Operator::TIMES);
	else if(strcmp(op.c_str(),ArithTerm::getNameOperator(Operator::DIV).c_str())==0)
		arith->setOperator(Operator::DIV);

}

void TermFactory::endArithTerm(){
	Term *at=terms[terms.size()-1];
	terms.pop_back();

	termsMap->addTerm(at);


	addTermsDependency(at);
}

void TermFactory::removeLastTerm(){
	if(terms.size()>0){
		terms[terms.size()-1]->popTerm();
	}else{
		termsInAtom.pop_back();
	}
}
