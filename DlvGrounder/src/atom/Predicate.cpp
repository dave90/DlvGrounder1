/*
 * Predicate.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "Predicate.h"
#include <cstring>
#include <boost/lexical_cast.hpp>

bool Predicate::operator ==(Predicate& p) {
	  return strcmp(p.getName().c_str(),this->getName().c_str())==0 && p.getArity()==this->getArity();
}

string Predicate::getNameToHash() {
	string hashString=name+"*"+boost::lexical_cast<string>(arity);
	return hashString;
}
