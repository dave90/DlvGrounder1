/*
 * ClassicalLiteral.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "ClassicalLiteral.h"
#include <sstream>
#include <iostream>
#include <string>

#include "../table/IdsManager.h"

using namespace std;

string ClassicalLiteral::getNameToHash(){
	ostringstream convert;
	convert << predicate;
	string name = convert.str()+"*";
	for (unsigned int i = 0; i < terms.size(); i++){
		convert << terms[i];
		name+=convert.str()+"*";
	}
	return name;
}


void ClassicalLiteral::print(){
	if(negative)
		cout<<"not ";
	if(hasMinus)
		cout<<"- ";
	cout<<IdsManager::getStringStrip(IdsManager::PREDICATE_ID_MANAGER,predicate);
	for (unsigned int i = 0; i < terms.size(); ++i){
		if(i==0)
			cout<<"(";
		cout<<IdsManager::getStringStrip(IdsManager::TERM_ID_MANAGER,terms[i]);
		if(i!=terms.size()-1)
			cout<<", ";
		else
			cout<<")";
	}
}

