/*
 * TermFactory.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef TERMFACTORY_H_
#define TERMFACTORY_H_

#include "term/VariableTerm.h"
#include "term/FunctionTerm.h"
#include "term/ConstantTerm.h"

#include "table/MapTermTable.h"

class TermFactory {
public:
	TermFactory();
	void createVariable(string variable,bool negative);
	void createConstant(string constant,bool negative);
	void createFunction(string name,bool negative);
	void endFunction();
	//TO-DELETE
	TermTable* getMap(){
		return termsMap;
	}
	~TermFactory();
private:
	vector<Term*> terms;
	TermTable *termsMap;
};


#endif /* TERMFACTORY_H_ */
