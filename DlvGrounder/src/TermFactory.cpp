/*
 * TermFactory.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "TermFactory.h"

#include "utility/Config.h"




 void TermFactory::createVariable(string& variable,bool negative) {
	Term *v=new VariableTerm(negative);

	index_object index=termsMap->addTerm(v,variable);

	addTermsDependency(index);
}



 void TermFactory::createConstant(string& constant,bool negative) {
	Term *c=new ConstantTerm(negative);

	index_object index=termsMap->addTerm(c,constant);

	addTermsDependency(index);

}

 void TermFactory::createConstant(unsigned& constant,bool negative) {
	Term *c=new ConstantTerm(negative);

	index_object index=termsMap->addTerm(c,constant);

	addTermsDependency(index);

}


 void TermFactory::endFunction() {
	Term *ft=terms[terms.size()-1];
	terms.pop_back();

	index_object index=termsMap->addTerm(ft);


	addTermsDependency(index);

}

 void TermFactory::addTermsDependency(index_object index) {
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

	index_object index=termsMap->addTerm(at);


	addTermsDependency(index);
}

void TermFactory::removeLastTerm(){
	if(terms.size()>0){
		terms[terms.size()-1]->popTerm();
	}else{
		termsInAtom.pop_back();
	}
}
