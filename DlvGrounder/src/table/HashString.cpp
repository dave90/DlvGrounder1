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
		// sx[0] max conflict
		for (int i = 0; i < length; i++) {
			hash+=sx[i]*pow(31.0,length-(i));
		}

		return hash;
}

size_t MurMurHashString::computeHash(string s) {
	const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;
	const int len=10;

	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *)s.c_str();
	const uint64_t * end = data + (len/8);

	while(data != end)
	{
		uint64_t k = *data++;

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch(len & 7)
	{
	case 7: h ^= uint64_t(data2[6]) << 48;break;
	case 6: h ^= uint64_t(data2[5]) << 40;break;
	case 5: h ^= uint64_t(data2[4]) << 32;break;
	case 4: h ^= uint64_t(data2[3]) << 24;break;
	case 3: h ^= uint64_t(data2[2]) << 16;break;
	case 2: h ^= uint64_t(data2[1]) << 8;break;
	case 1: h ^= uint64_t(data2[0]);
	        h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}
