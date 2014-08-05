/*
 * ClassicalLiteral.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "ClassicalLiteral.h"

#include <iostream>

#include "../table/IdsManager.h"
#include "../table/HashVecInt.h"
#include "../table/TermTable.h"

using namespace std;

size_t ClassicalLiteral::getHash() const{
	return HashVecInt::getHashVecIntFromConfig()->computeHash(terms);
}


void ClassicalLiteral::print(){
	print(predicate,terms,negative,hasMinus);
}

void ClassicalLiteral::print(index_object predicate,vector<index_object>& terms,bool negative,bool hasMinus){
	TermTable*tb=TermTable::getInstance();

	if(negative)
		cout<<"not ";
	if(hasMinus)
		cout<<"- ";
	cout<<IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,predicate);
	for (unsigned int i = 0; i < terms.size(); ++i){
		if(i==0)
			cout<<"(";
		tb->getTerm(terms[i])->print();
		if(i!=terms.size()-1)
			cout<<",";
		else
			cout<<")";
	}
}


bool ClassicalLiteral::operator==(const Atom& a) {

	if(predicate!=a.getPredicate().second)return false;
	if(terms.size()!=a.getTermsSize())return false;
	for(unsigned int i=0;i<terms.size();i++)
		if(terms[i]!=a.getTerm(i).second)
			return false;

	return true;

}

