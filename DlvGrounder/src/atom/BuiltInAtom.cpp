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

	if(binop==Binop::EQUAL)
		return *termTable->getTerm(firstTerm)==*termTable->getTerm(secondTerm);
	if(binop==Binop::UNEQUAL)
		return !(*termTable->getTerm(firstTerm)==*termTable->getTerm(secondTerm));
//TODO Following Operators in Term
//	if(binop==Binop::LESS)
//		return *termTable->getTerm(firstTerm)<*termTable->getTerm(secondTerm);
//	if(binop==Binop::LESS_OR_EQ)
//			return *termTable->getTerm(firstTerm)<=*termTable->getTerm(secondTerm);
//	if(binop==Binop::GREATER)
//			return *termTable->getTerm(firstTerm)>*termTable->getTerm(secondTerm);
//	if(binop==Binop::GREATER_OR_EQ)
//			return *termTable->getTerm(firstTerm)>=*termTable->getTerm(secondTerm);
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

