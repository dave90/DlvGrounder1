/*
 * ConstantTerm.h
 *
 *  Created on: 27/feb/2014
 *      Author: Davide
 */

#ifndef CONSTANTTERM_H_
#define CONSTANTTERM_H_

#include <stdlib.h>

#include "Term.h"


/*
 *  Represent a constant term (string,number...)
 *
 *  The constant is in IdsManager for avoiding duplication
 */

class ConstantTerm: public Term {
public:
	ConstantTerm():Term(){};
	ConstantTerm(bool negative):Term(negative){};
	ConstantTerm(bool negative,index_object index):Term(negative,index){};

	virtual TermType getType()const{return TermType::CONSTANT;};
	virtual bool contain(TermType type){if(TermType::CONSTANT==type)return true;return false;};
	virtual bool isGround(){return true;}


	/// Return the value of the constant
	virtual Term* substitute(map_term_term& substritutionTerm){
		auto find_it=substritutionTerm.find(this);
		if(find_it!=substritutionTerm.end())
			return (*find_it).second;
		return this;
	};

	virtual bool match(Term* termToMatch,map_term_term& varAssignment);

	virtual void print(){
		cout<<getName();
	}

};

class NumericConstantTerm: public ConstantTerm{
public:
	NumericConstantTerm(bool negative,int n):ConstantTerm(negative),numeric_constant(n){};
	virtual string getName()const{return boost::lexical_cast<string>(numeric_constant);};
	virtual int getConstantValue(){return numeric_constant;};


private:
	int numeric_constant;
};

class StringConstantTerm: public ConstantTerm{
public:
	StringConstantTerm(bool negative,string n):ConstantTerm(negative),string_constant(n){};
	virtual string getName()const{return string_constant;};

private:
	string string_constant;
};



#endif /* CONSTANTTERM_H_ */
