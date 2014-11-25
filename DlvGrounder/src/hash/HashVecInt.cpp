/*
 * HashVecInt.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: david
 */

#include "HashVecInt.h"
#include "../utility/Config.h"

HashVecInt *HashVecInt::hashInt;

HashVecInt* HashVecInt::getHashVecIntFromConfig() {
	if (hashInt == nullptr){
		if(Config::getInstance()->getHashType()==HashType::BOOST_HASH)
			hashInt = new BoostCombineHashVecInt;
		else
			// Default select Java
			hashInt = new JavaHashVecInt;
	}

	return hashInt;
}
