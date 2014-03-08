/*
 * NafLiteral.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "NafLiteral.h"

bool NafLiteral::isNegative() const {
	return negative;
}

void NafLiteral::setNegative(bool negative) {
	this->negative = negative;
}
