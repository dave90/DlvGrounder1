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
#include "utility/IndexDefinition.h"



class TermFactory {

public:
	// Set table to hash stl table of terms
	TermFactory();
	// Create a Variable term
	void createVariable(string& variable,bool negative);
	//Create String Constant term
	void createConstant(string& constant,bool negative);
	//Create Integer Constant term
	void createConstant(unsigned int& constant,bool negative);
	//Create Function Term and push it in a queue
	void createFunction(string& name,bool negative);
	// Put the Function term in the tail of a queue in the Table Term
	void endFunction();
	//Add Arithmetic term
	void addArithTerm();
	///Remove last term
	void removeLastTerm();
	/// Set operator of ArithTerm
	void setArithOperator(string& op);
	/// End arith term
	void endArithTerm();

	//if terms.size>0 Add term in functional term otherwise return index
	void addTermsDependency(index_object index);

	/// Return the terms in atom
	vector<index_object>& getTermsInAtom(){return termsInAtom;};
	/// Remove Terms in atom
	void clearTermsInAtom(){termsInAtom.clear();};


	~TermFactory(){};
private:
	/*
	 *  Store the order of the term function encountered for nested function
	 *
	 */
	vector<Term*> terms;
	/*
	 * Terms in atom
	 */
	vector<index_object> termsInAtom;


	/*
	 *  Table of the all terms, TEMPORARY POSITION
	 */
	TermTable *termsMap;

};


#endif /* TERMFACTORY_H_ */
