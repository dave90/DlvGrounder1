/*
 * AggregateAtom.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "AggregateAtom.h"
#include <sstream>
using namespace std;

string AggregateAtom::getNameToHash(){
	ostringstream convert;
	convert << firstCompareTerm;
	string name=convert.str();
	convert << firstBinop;
	name+="*"+convert.str();
	convert << aggregateFunction;
	name+="*"+convert.str()+"*";
	for (unsigned int i = 0; i < aggregateElements.size(); i++) {
		vector<index_object> v=aggregateElements[i].getNafLiterals();
		for (unsigned int j = 0; j < v.size(); j++) {
			convert << v[i];
			name+=convert.str()+"*";
		}
	}
	for (unsigned int i = 0; i < aggregateElements.size(); i++) {
		vector<index_object> v=aggregateElements[i].getTerms();
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
