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
 *  Represent a variable term
 */
class VariableTerm: public Term {
public:
	VariableTerm(){};
	VariableTerm(bool negative):Term(negative){};
	VariableTerm(index_object index){setIndex(index);};
	virtual string getName(){return IdsManager::getString(IdsManager::TERM_ID_MANAGER,getIndex());};
	virtual bool isAnonymous(){return getName().compare("_")==0;};
	virtual bool isVariable(){return true;};
	virtual string getNameToHash(){	return getName();};
};

#endif /* VARIABLETERM_H_ */
