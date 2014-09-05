/*
 * Atom.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "Atom.h"

#include "../term/ConstantTerm.h"
#include "../table/TermTable.h"


Atom* Atom::ground(unordered_map<index_object, index_object>& substritutionTerm){
	Atom *substitute_atom=substitute(substritutionTerm);
	TermTable *termTable=TermTable::getInstance();
	vector<index_object> terms_arith_calculated;
	index_object index;

	for(unsigned int i=0;i<substitute_atom->getTermsSize();i++){
		index_object term=substitute_atom->getTerm(i).second;
		if(termTable->getTerm(term)->isArith()){
			/// Calculate the value of arithmetic term and add in terms table
			index=termTable->getTerm(term)->calculate();

		}else
			index=term;

		terms_arith_calculated.push_back(index);
	}

	substitute_atom->setTerms(terms_arith_calculated);
	return substitute_atom;
};

unordered_set<index_object> Atom::getVariable(){
	unordered_set<index_object> variables;
	for(index_object term:terms)
		TermTable::getInstance()->getTerm(term)->getVariable(variables);
	return variables;
}

