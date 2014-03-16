/*
 * BuiltInAtom.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "BuiltInAtom.h"
#include <sstream>

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

string BuiltInAtom::getNameToHash(){
	ostringstream convert;
	convert << firstTerm;
	string name=convert.str();
	convert << binop;
	name+="*"+convert.str();
	convert << secondTerm;
	name+="*"+convert.str();
	return name;
}
