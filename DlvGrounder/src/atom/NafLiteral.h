/*
 * NafLiteral.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 *
 *      Deprecated
 */

#ifndef NAFLITERAL_H_
#define NAFLITERAL_H_

#include "Atom.h"

//This class is not used anymore

class NafLiteral: public Atom {
public:
	NafLiteral() : negative(false){};
	NafLiteral(bool negative): negative(negative){};

	bool isNegative() const;
	void setNegative(bool negative);

	virtual ~NafLiteral();

private:
	bool negative;
};

#endif /* NAFLITERAL_H_ */
