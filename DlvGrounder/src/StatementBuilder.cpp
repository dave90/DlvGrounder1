/*
 * StatementBuilder.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "StatementBuilder.h"

StatementBuilder::StatementBuilder() {
	 statement = 0;
	 weak = 0;
	 constraint = 0;
	 disjunction = 0;
	 choice = 0;
	 aggregate = 0;
	 literal = 0;
	 term = 0;

	 negativeTerm=false;
	 id="";
}

void StatementBuilder::printStats() {
	cout << "Statement: " << statement << endl;
	cout << "Constraint: " << constraint << endl;
	cout << "Weak: " << weak << endl;
	cout << "Disjunction: " << disjunction << endl;
	cout << "Choice: " << choice << endl;
	cout << "Aggregate: " << aggregate << endl;
	cout << "Literal: " << literal << endl;
	cout << "Term: " << term << endl;

	TermTable *tm=termsFactory.getMap();
	cout<<"Size Map "<<tm->getSize()<<endl;
	boost::timer t1;
	for(unsigned long i=0;i<tm->getSize();i++){
		tm->getTerm(i);
	}
	cout<<"Time for visit: "<<t1.elapsed()<<endl;

}

void StatementBuilder::addStatement() {
	statement++;
}

void StatementBuilder::addWeak() {
	weak++;
}

void StatementBuilder::addConstraint() {
}

void StatementBuilder::addChoiche() {
	choice++;
}

void StatementBuilder::addDisjunction() {
	disjunction++;
}

void StatementBuilder::addAggregate() {
	aggregate++;
}

void StatementBuilder::addLiteral() {

	literal++;
}


//void StatementBuilder::addTerm() {
//	if(variable.size()>0){
//		termsFactory.createVariable(variable,negativeTerm);
//	}else if(strcmp("+number",id.c_str())==0){
//		ostringstream convert;
//		convert << number;
//		id=convert.str();
//		termsFactory.createConstant(id,negativeTerm);
//	}else if(id.size()>0){
//		termsFactory.createConstant(id,negativeTerm);
//	}
//
//
//	resetTerm();
//
//	term++;
//}

void StatementBuilder::resetTerm(){
	 id="";
	 negativeTerm=false;
}

void StatementBuilder::addVariable(string & name) {
	termsFactory.createVariable(name,negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addId(string & name) {
	termsFactory.createConstant(name,negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addNumber(int & name) {
	ostringstream convert;
	convert << name;
	string id=convert.str();
	termsFactory.createConstant(id,negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addTermFunction() {
	termsFactory.createFunction(id,negativeTerm);
	resetTerm();

	term++;
}

void StatementBuilder::addNameFunction(string & name) {
	id=name;
}

void StatementBuilder::endTermFunction() {
	termsFactory.endFunction();
	resetTerm();
}

void StatementBuilder::setNegativeTerm() {
	negativeTerm=true;
}
