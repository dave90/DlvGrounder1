/*
 * Atom.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "Atom.h"

#include "../term/ConstantTerm.h"
#include "../table/TermTable.h"


Atom* Atom::ground(map_term_term& substritutionTerm){
	Atom *substitute_atom=substitute(substritutionTerm);

	for(unsigned int i=0;i<substitute_atom->getTermsSize();i++){
		Term* term=substitute_atom->getTerm(i);
		if(term->contain(TermType::ARITH))
			/// Calculate the value of arithmetic term and add in terms table
			substitute_atom->setTerm(i,term->calculate());

	}

	return substitute_atom;
};

set_term Atom::getVariable(){
	set_term variables;
	for(auto term:terms)
		term->getVariable(variables);
	return variables;
}

