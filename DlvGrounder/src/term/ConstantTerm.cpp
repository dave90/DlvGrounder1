/*
 * ConstantTerm.cpp
 *
 *  Created on: 27/feb/2014
 *      Author: Davide
 */

#include "ConstantTerm.h"
#include "../table/TermTable.h"

bool ConstantTerm::match(Term* termToMatch,map_term_term& varAssignment) {
	// If is anonymus return true, if is variable put a value of constant in binds
	// if is constant compare the value of two constant term
	// If is a variable and in assignment exist this variable , treat it like constant


	if (termToMatch->getType()==TermType::ANONYMOUS) return true;
	if (termToMatch->getType()==TermType::VARIABLE) {

		auto find_it=varAssignment.find(termToMatch);
		if(find_it!=varAssignment.end()){
			if( (*find_it).second->getIndex() == index)
				return true;
			else
				return false;
		}

		varAssignment.insert( { termToMatch, this });
		return true;
	}
	if (termToMatch->getType()==TermType::CONSTANT && termToMatch->getIndex() == index) return true;

	return false;
}
;
