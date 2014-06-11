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
 */
class ConstantTerm: public Term {
public:
	ConstantTerm(){};
	ConstantTerm(bool negative):Term(negative){};
	ConstantTerm(index_object index){setIndex(index);};
	virtual string getName(){return IdsManager::getString(IdsManager::TERM_ID_MANAGER,getIndex());};
	virtual string getNameToHash(){return getName();};
	//FIXME take the integer not the general find of IdManager
	virtual double calculate(){	return atof(getName().c_str());};
};

#endif /* CONSTANTTERM_H_ */
