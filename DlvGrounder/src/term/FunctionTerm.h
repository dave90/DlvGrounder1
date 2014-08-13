/*
 * FunctionTerm.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef FUNCTIONTERM_H_
#define FUNCTIONTERM_H_


#include "Term.h"

#include "../table/TermTable.h"


using namespace std;

/*
 *  Represent a functional term.
 */
class FunctionTerm: public Term {
public:
	FunctionTerm(string& name,bool negative):Term(negative),name(name){};
	FunctionTerm(string v,index_object index):name(v){setIndex(index);};
	virtual string getName(){return name;};
	virtual void setName(string& name){this->name=name;};
	virtual void addTerm(index_object termIndex){terms.push_back(termIndex);};
	virtual void popTerm(){terms.pop_back();};
	virtual bool isFunctionalTerm(){return true;};
	virtual vector<index_object> getTerms(){return terms;};

	/// If one term are variable then return false, else true
	virtual bool isVariable(){
		for(index_object term:terms)
			if(TermTable::getInstance()->getTerm(term)->isVariable())return false;
		return true;
	}

	virtual bool isConstant(){
		for(index_object term:terms)
			if(!TermTable::getInstance()->getTerm(term)->isConstant())return false;
		return true;
	};

	virtual void getVariable(unordered_set<index_object>& variables){
		for(index_object term:terms)
			TermTable::getInstance()->getTerm(term)->getVariable(variables);
	};


	virtual index_object substitute(unordered_map<index_object, index_object>& substritutionTerm);
	/// Return the name of the function concatenated with '*' and the id of the composites term
	virtual string getNameToHash();
	/// Match a function with given id of term, compare the constant term and put in binds
	/// a value of the variable term present in termToMatch
	/// Return true if constant term are equal, else false
	virtual bool match(index_object termToMatch,unordered_map<index_object, index_object>& varAssignment);
	virtual void print();
private:
	/**
	 *   The name of a function
	 */
	string name;

	/**
	 *  All the index of the terms inside the function
	*/
	vector<index_object> terms;
};

#endif /* FUNCTIONTERM_H_ */
