/*
 * Atom.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "Atom.h"
#include <iostream>
using namespace std;

Atom::Atom(unsigned long predicate){
	index=0;
	setPredicate(predicate);
}

bool Atom::operator==(const Atom& a){
	return this->index==a.index;
}

void Atom::print(){
	cout<<"Atom\t"<<"Index: "<<index<<endl;
}

