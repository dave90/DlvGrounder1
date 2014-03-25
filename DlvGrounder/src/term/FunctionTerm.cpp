/*
 * FunctionTerm.cpp
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#include "FunctionTerm.h"
#include <sstream>

FunctionTerm::FunctionTerm() {

}


string FunctionTerm::getNameToHash() {
	string hashString=name+"*";

	string number;
	for(unsigned int i=0;i<terms.size();i++){
		stringstream ss;
		ss << terms[i];
		number = ss.str();

		hashString+=number+"*";
	}

	return hashString;
}

void FunctionTerm::print() {
	cout<<"Function Term "<<name<<" index: "<<getIndex()<<" Negation: "<<isNegative()<<" "<<" Terms:";
	for(unsigned int i=0;i<terms.size();i++)
		cout<<terms[i]<<" ";
	cout<<endl;
}

