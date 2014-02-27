/*
 * TermFactory.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef TERMFACTORY_H_
#define TERMFACTORY_H_

#include "VariableTerm.h"
#include "FunctionTerm.h"
#include "ConstantTerm.h"
#include "MapTermTable.h"

class TermFactory {
public:
	TermFactory();
	void createVariable(string variable);
	void createConstant(string constant);
	void createFunction(string name);
	void endFunction();
	//TO-DELETE
	MapTermTable getMap(){
		return termsMap;
	}
private:
	vector<Term*> terms;
	MapTermTable termsMap;
};

#endif /* TERMFACTORY_H_ */
