/*
 * FunctionTerm.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "FunctionTerm.h"

#include <sstream>
#include <boost/lexical_cast.hpp>



string FunctionTerm::getNameToHash() {
	string hashString=name+"*";

	string number;
	for(unsigned int i=0;i<terms.size();i++){
		hashString+=boost::lexical_cast<string>(terms[i])+"*";
	}

	return hashString;
}

void FunctionTerm::print() {
	cout<<IdsManager::getStringStrip(IdsManager::TERM_ID_MANAGER,getIndex())<<"(";
	for(unsigned int i=0;i<terms.size();i++){
		if(i!=0)
			cout<<",";
		TermTable::getInstance()->getTerm(terms[i])->print();
	}
	cout<<")";
}

