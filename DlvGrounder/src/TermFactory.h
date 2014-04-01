/*
 * TermFactory.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef TERMFACTORY_H_
#define TERMFACTORY_H_

/*
 *  TermFactory  create a Term and push in the Term Table
 */

#include "term/VariableTerm.h"
#include "term/FunctionTerm.h"
#include "term/ConstantTerm.h"
#include "term/ArithTerm.h"
#include "table/HashTermTable.h"
#include "table/BoostTermTable.h"


class TermFactory {
public:
	// Set table to hash stl table of terms
	TermFactory();
	void setTableType();
	// Create a Variable term
	unsigned long createVariable(string variable,bool negative);
	//Create Constant term
	unsigned long createConstant(string constant,bool negative);
	//Create Function Term and push it in a queue
	void createFunction(string name,bool negative);
	// Put the Function term in the tail of a queue in the Table Term
	unsigned long endFunction();
	//Add Arithmetic term
	unsigned long addArithTerm(string op);

	//if terms.size>0 Add term in functional term otherwise return index
	unsigned long addTermsDependency(unsigned long index);

	//TO-DELETE decide where put tha Term Table
	TermTable* getMap(){
		return termsMap;
	}

	~TermFactory();
private:
	/*
	 *  Store the order of the term function encountered for nested function
	 *
	 */
	vector<Term*> terms;

	/*
	 *  Table of the all terms, TEMPORARY POSITION
	 */
	TermTable *termsMap;

	/*
	 *  Last term putted in the table
	 */
	unsigned long lastTerm;

	/*
	 *  True if meet atirhmetic term
	 */
	bool arith;
};


#endif /* TERMFACTORY_H_ */
