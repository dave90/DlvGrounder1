/*
 * ConstantTerm.cpp
 *
 *  Created on: 27/feb/2014
 *      Author: Davide
 */

#include "ConstantTerm.h"
#include "../table/TermTable.h"

bool ConstantTerm::match(index_object termToMatch, unordered_map<index_object, index_object>& varAssignment) {
	// If is anonymus return true, if is variable put a value of constant in binds
	// if is constant compare the value of two constant term

	Term *term = TermTable::getInstance()->getTerm(termToMatch);

	if (term->isAnonymous()) return true;
	if (term->isVariableTerm()) {
		varAssignment.insert( { termToMatch, getIndex() });
		return true;
	} else if (term->isConstantTerm() && term->getIndex() == getIndex()) return true;

	return false;
}
;
