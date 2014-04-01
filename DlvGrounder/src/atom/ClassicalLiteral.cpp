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
	string print;
	for (unsigned int i = 0; i < terms.size(); ++i)
		print+=terms[i]+", ";
	cout<<"Classical Atom: "<<predicate<<"("<<print<<")\n";
}

