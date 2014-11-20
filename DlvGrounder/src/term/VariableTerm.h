/*
 * VariableTerm.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef VARIABLETERM_H_
#define VARIABLETERM_H_


#include "Term.h"

using namespace std;

/*
 * Represent a term variable
 *
 * The variable is in IdsManager for avoiding duplication
 */
class VariableTerm: public Term {
public:
	VariableTerm():Term(),isAnonymous(0){};
	VariableTerm(bool negative):Term(negative),isAnonymous(0){};
	VariableTerm(bool negative,index_object index,string& name):VariableTerm(negative,name),index(index){};
	VariableTerm(bool negative,string& name):Term(negative){
		if(name.compare("_")==0)
			isAnonymous=true;
		else
			isAnonymous=false;
	};

	virtual string getName(){return name;};
	virtual void setName(const string& name){
		this->name=name;
		if(name.compare("_")==0)
			isAnonymous=true;
		else
			isAnonymous=false;
	};

	//Return the type of term
	virtual TermType getType(){if(isAnonymous)return TermType::ANONYMOUS;return TermType::VARIABLE;};
	//Return true if contain a term of the given type
	virtual bool contain(TermType type){
		if(TermType::VARIABLE==type || (TermType::ANONYMOUS==type && isAnonymous))
			return true;
		return false;
	};


	virtual void getVariable(set_term& variables){if(!isAnonymous)variables.insert(this);};
	virtual size_t hash(){	return HashString::getHashStringFromConfig()->computeHash(name);};
	virtual Term* substitute(map_term_term& substritutionTerm){
		auto find_it=substritutionTerm.find(this);
		if(find_it!=substritutionTerm.end())
			return (*find_it).second;
		return this;
	};

	virtual void print(){
		cout<<name;
	}

private:
	/**
	 *   The name of a variable
	 */
	string name;
	/*
	 *  True if is an anonymous variable ''_''
	 */
	bool isAnonymous;

};

#endif /* VARIABLETERM_H_ */
