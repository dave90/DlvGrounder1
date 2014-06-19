/*
 * HashVecInt.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: david
 */

#include "HashVecInt.h"

HashVecInt *HashVecInt::hashInt;

HashVecInt* HashVecInt::getHashVecIntFromConfig() {
	//FIXME add config switch
	if (hashInt == nullptr)
		hashInt = new JavaHashVecInt;
	return hashInt;
}
