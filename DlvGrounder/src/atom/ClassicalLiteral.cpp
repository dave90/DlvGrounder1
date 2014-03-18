/*
 * ClassicalLiteral.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "ClassicalLiteral.h"
#include <sstream>

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

