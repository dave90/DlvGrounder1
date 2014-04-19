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
	long createVariable(string variable,bool negative);
	//Create Constant term
	long createConstant(string constant,bool negative);
	//Create Function Term and push it in a queue
	void createFunction(string name,bool negative);
	// Put the Function term in the tail of a queue in the Table Term
	long endFunction();
	//Add Arithmetic term
	void addArithTerm();
	/// Set operator of ArithTerm
	void setArithOperator(string op);
	/// End arith term
	 long endArithTerm();

	//if terms.size>0 Add term in functional term otherwise return index
	long addTermsDependency(unsigned long index);


	TermTable*  getTermTable(){return termsMap;};

	~TermFactory(){};
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

};


#endif /* TERMFACTORY_H_ */
