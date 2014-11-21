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
#include "term/ArithTerm.h"
#include "utility/IndexDefinition.h"



/**
 *  Manage the creation of the terms
 *  Group all term encountered like a terms in a atom for simplify
 *  the manage of terms in atom
 */
class TermFactory {

public:
	TermFactory(){termsMap=TermTable::getInstance();};
	/// Create a Variable term
	void createVariable(string& variable,bool negative);
	/// Create string constant term
	void createConstant(string& constant,bool negative);
	/// Create integer constant term
	void createConstant(unsigned int& constant,bool negative);
	/// Create function term (all terms henceforth met are added in terms )
	void createFunction(string& name,bool negative){
		Term *ft=new FunctionTerm(name,negative);
		terms.push_back(ft);
	};
	/// End functional term (all terms henceforth met are added in termsInAtom if there isn't another dependency)
	void endFunction();
	/// Add arithmetic term (all terms henceforth met are added in terms )
	void addArithTerm(){
		Term *t=new ArithTerm;
		terms.push_back(t);
	}
	/// Remove last term
	void removeLastTerm();
	/// Set operator of ArithTerm
	void setArithOperator(string& op);
	/// End arith term (all terms henceforth met are added in termsInAtom if there isn't another dependency)
	void endArithTerm();

	/// If a functional term or arith term hash encontered all the
	/// the terms are added in terms else added in termsInAtom
	void addTermsDependency(Term* index);

	/// Return the terms in atom
	vector<Term*>& getTermsInAtom(){return termsInAtom;};
	/// Remove Terms in atom
	void clearTermsInAtom(){termsInAtom.clear();};


	~TermFactory(){};
private:
	/*
	 *  Store the order of the term function or arith terms encountered for nested dependency
	 *
	 */
	vector<Term*> terms;
	/*
	 * Terms in atom
	 */
	vector<Term*> termsInAtom;


	/*
	 *  Table of the all terms,
	 */
	TermTable *termsMap;

};


#endif /* TERMFACTORY_H_ */
