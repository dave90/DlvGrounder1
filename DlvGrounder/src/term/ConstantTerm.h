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
	ConstantTerm():Term(0){};
	ConstantTerm(bool negative):Term(negative){};
	virtual string getName(){return IdsManager::getString(IdsManager::TERM_ID_MANAGER,getIndex());};
	virtual string getNameToHash(){return getName();};
	virtual bool isConstant(){return true;};
	virtual bool isConstantTerm(){return true;};
	//FIXME we can optimize adding the method in IdsManager for returning the integer value
	/// Return the value of the constant
	virtual double calculate(){	return atof(getName().c_str());};
	virtual index_object substitute(unordered_map<index_object, index_object>& substritutionTerm){
		for(auto term_value:substritutionTerm)
			if(term_value.first == getIndex()){
				return term_value.second;
			}
		return getIndex();
	};

	virtual bool match(index_object termToMatch,unordered_map<index_object, index_object>& varAssignment);


};

#endif /* CONSTANTTERM_H_ */
