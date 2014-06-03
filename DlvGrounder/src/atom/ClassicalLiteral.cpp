/*
 * ClassicalLiteral.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "ClassicalLiteral.h"

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

#include "../table/IdsManager.h"

using namespace std;

string ClassicalLiteral::getNameToHash(){

	return getNameToHash(predicate,terms);

}


void ClassicalLiteral::print(TermTable *tb){
	if(negative)
		cout<<"not ";
	if(hasMinus)
		cout<<"- ";
	cout<<IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,predicate);
	for (unsigned int i = 0; i < terms.size(); ++i){
		if(i==0)
			cout<<"(";
		tb->getTerm(terms[i])->print(tb);
		if(i!=terms.size()-1)
			cout<<", ";
		else
			cout<<")";
	}
}

string ClassicalLiteral::getNameToHash(unsigned long predicate, vector<unsigned long>& terms) {

	string name = boost::lexical_cast<string>(predicate)+"*";
	for (unsigned int i = 0; i < terms.size(); i++){
		name+=boost::lexical_cast<string>(terms[i])+"*";
	}
	return name;

}
