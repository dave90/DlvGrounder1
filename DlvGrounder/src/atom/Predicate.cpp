/*
 * Predicate.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "Predicate.h"
#include <boost/lexical_cast.hpp>

#include <sstream>

void Predicate::setArity(int arity){
	this->arity=arity;
}

int Predicate::getArity(){
	return this->arity;
}

void Predicate::setName(string name){
	this->name=name;
}

string Predicate::getName(){
	return this->name;
}

index_object Predicate::getIndex() const {
	return index;
}

void Predicate::setIndex(index_object index) {
	this->index = index;
}

bool Predicate::operator ==(Predicate& p) {
	  return strcmp(p.getName().c_str(),this->getName().c_str())==0 && p.getArity()==this->getArity();
}

string Predicate::getNameToHash() {
	string hashString=name+"*"+boost::lexical_cast<string>(arity);
	return hashString;
}
