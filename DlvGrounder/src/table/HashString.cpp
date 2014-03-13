/*
 * HashString.cpp
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#include "HashString.h"

#include <boost/functional/hash.hpp>
#include <math.h>



size_t STLHashString::computeHash(string s){
	  hash<string> str_hash;
	  return str_hash(s);
}

size_t BOOSTHashString::computeHash(string s) {
	 boost::hash<string> str_hash;

	 return str_hash(s);
}

size_t JavaHashString::computeHash(string s) {
		const char* sx=s.c_str();
		int length=s.length();
		size_t hash=0;
		for (int i = 0; i < length; i++) {
			hash+=sx[0]*pow(31.0,length-(i));
		}

		return hash;
}
