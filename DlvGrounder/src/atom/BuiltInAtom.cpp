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

bool BuiltInAtom::evaluate(){
	index_object firstTerm=terms[0];
	index_object secondTerm=terms[1];

	TermTable *termTable=TermTable::getInstance();

	// Take the value of firstBinop and SecondBinop
	double value1= *termTable->getTerm(firstTerm)->calculate();
	double value2= *termTable->getTerm(secondTerm)->calculate();

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

