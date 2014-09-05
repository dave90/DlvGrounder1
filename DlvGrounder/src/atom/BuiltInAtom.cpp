/*
 * BuiltInAtom.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "BuiltInAtom.h"

#include <iostream>

#include "../table/IdsManager.h"
#include "../table/TermTable.h"
#include "../table/HashVecInt.h"

unsigned int calculateValueTerm(index_object term){
	return atof(TermTable::getInstance()->getTerm( TermTable::getInstance()->getTerm(term)->calculate() )->getName().c_str());
}

index_object getIndexOfCalculateValueTerm(index_object term){
	return TermTable::getInstance()->getTerm(term)->calculate();
}

bool BuiltInAtom::evaluate(unordered_map<index_object, index_object>& substitutionTerm){
	index_object firstTerm=terms[0];
	index_object secondTerm=terms[1];

	TermTable *termTable=TermTable::getInstance();

	// If there is equal and variable assign that value
	if(binop==Binop::EQUAL && (termTable->getTerm(firstTerm)->isVariableTerm() || termTable->getTerm(secondTerm)->isVariableTerm() )){
		if(termTable->getTerm(firstTerm)->isVariableTerm())
			substitutionTerm.insert({firstTerm,getIndexOfCalculateValueTerm(secondTerm)});
		else
			substitutionTerm.insert({secondTerm,getIndexOfCalculateValueTerm(firstTerm)});

		return true;
	}

	// Take the value of firstBinop and SecondBinop
	unsigned int value1= calculateValueTerm(firstTerm);
	unsigned int value2= calculateValueTerm(secondTerm);

	if(binop==Binop::EQUAL)
		return value1==value2;
	if(binop==Binop::UNEQUAL)
		return value1!=value2;
	if(binop==Binop::LESS)
		return value1<value2;
	if(binop==Binop::LESS_OR_EQ)
		return value1<=value2;
	if(binop==Binop::GREATER)
		return value1>value2;
	if(binop==Binop::GREATER_OR_EQ)
		return value1>=value2;

	return false;
}

size_t BuiltInAtom::getHash() const{
	return HashVecInt::getHashVecIntFromConfig()->computeHash(terms);
}

bool BuiltInAtom::operator==(const Atom& a) {
	index_object firstTerm=terms[0];
	index_object secondTerm=terms[1];
	TermTable *termTable=TermTable::getInstance();

	if(!(*(termTable->getTerm(firstTerm)) == *(termTable->getTerm(a.getTerm(0).second)))) return false;
	if(!(*(termTable->getTerm(secondTerm)) == *(termTable->getTerm(a.getTerm(1).second)))) return false;
	if(binop != a.getBinop()) return false;

	return true;
}

void BuiltInAtom::print(){
	TermTable*tb=TermTable::getInstance();
	index_object firstTerm=terms[0];
	index_object secondTerm=terms[1];

	tb->getTerm(firstTerm)->print();
	if(binop==Binop::EQUAL)
		cout<<"=";
	if(binop==Binop::UNEQUAL)
		cout<<"!=";
	if(binop==Binop::LESS)
		cout<<"<";
	if(binop==Binop::LESS_OR_EQ)
		cout<<"<=";
	if(binop==Binop::GREATER)
		cout<<">";
	if(binop==Binop::GREATER_OR_EQ)
		cout<<">=";
	tb->getTerm(secondTerm)->print();
}

Atom* BuiltInAtom::substitute(unordered_map<index_object, index_object>& substritutionTerm) {
	vector<index_object> terms_substitute;
	TermTable *termTable=TermTable::getInstance();
	for(index_object term:terms){
		terms_substitute.push_back( termTable->getTerm(term)->substitute(substritutionTerm) );
	}
	return new BuiltInAtom(binop,negative,terms_substitute);
}
