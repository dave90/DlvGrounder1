/*
 * BuiltInAtom.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "BuiltInAtom.h"

#include <sstream>
#include <iostream>

#include "../table/IdsManager.h"

bool BuiltInAtom::evaluate(){ //TODO Operatori in Term
	if(binop==Binop::EQUAL)
		return termTable->getTerm(firstTerm)==termTable->getTerm(secondTerm);
	if(binop==Binop::UNEQUAL)
		return termTable->getTerm(firstTerm)!=termTable->getTerm(secondTerm);
	if(binop==Binop::LESS)
		return termTable->getTerm(firstTerm)<termTable->getTerm(secondTerm);
	if(binop==Binop::LESS_OR_EQ)
			return termTable->getTerm(firstTerm)<=termTable->getTerm(secondTerm);
	if(binop==Binop::GREATER)
			return termTable->getTerm(firstTerm)>termTable->getTerm(secondTerm);
	if(binop==Binop::GREATER_OR_EQ)
			return termTable->getTerm(firstTerm)>=termTable->getTerm(secondTerm);
	return false;
}

//TODO
size_t BuiltInAtom::getHash() const{
	return 0;
}

bool BuiltInAtom::operator ==(const Atom& a) {
	if(firstTerm != a.getFirstTerm()) return false;
	if(secondTerm != a.getSecondTerm()) return false;
	if(binop != a.getBinop()) return false;

	if(negative != a.isNegative()) return false;

	return true;

}

void BuiltInAtom::print(TermTable*tb){
	tb->getTerm(firstTerm)->print(tb);
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
	tb->getTerm(secondTerm)->print(tb);
}

