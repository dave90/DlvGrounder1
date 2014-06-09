/*
 * Choice.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "Choice.h"
#include <sstream>
using namespace std;

string Choice::getNameToHash(){
	ostringstream convert;
	convert << firstCompareTerm;
	string name=convert.str();
	convert << firstBinop;
	name+="*"+convert.str();
	for (unsigned int i = 0; i < choiceElements.size(); i++) {
		vector<index_object> v=choiceElements[i];
		for (unsigned int j = 0; j < v.size(); j++) {
			convert << v[i];
			name+=convert.str()+"*";
		}
	}
	convert << secondBinop;
	name+="*"+convert.str();
	convert << secondCompareTerm;
	name+="*"+convert.str()+"*";
	return name;
}
