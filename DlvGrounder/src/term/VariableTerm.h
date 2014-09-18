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
	VariableTerm():Term(0){};
	VariableTerm(bool negative):Term(negative){};
	virtual string getName(){return IdsManager::getString(IdsManager::TERM_ID_MANAGER,getIndex());};
	virtual bool isAnonymous(){return getName().compare("_")==0;};
	virtual bool containsAnonymous(){return isAnonymous();};
	/// If isn't anonymus return true else false
	virtual bool isVariable(){if(!isAnonymous())return true;return false;};
	virtual bool isVariableTerm(){return true;};

	virtual void getVariable(unordered_set<index_object>& variables){if(!isAnonymous())variables.insert(getIndex());};
	virtual string getNameToHash(){	return getName();};
	virtual index_object substitute(unordered_map<index_object, index_object>& substritutionTerm){
		for(auto term_value:substritutionTerm)
			if(term_value.first == getIndex()){
				return term_value.second;
			}
		return getIndex();
	};
};

#endif /* VARIABLETERM_H_ */
