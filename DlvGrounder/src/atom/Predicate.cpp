/*
 * Predicate.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "Predicate.h"

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

unsigned long Predicate::getIndex() const {
	return index;
}

void Predicate::setIndex(unsigned long index) {
	this->index = index;
}
