/*
 * HashString.cpp
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#include "HashString.h"

#include "../utility/Config.h"

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
			hash+=sx[i]*pow(31.0,length-(i));
		}

		return hash;
}

size_t PerlBernsteinHashString::computeHash(string s) {
		const char* sx=s.c_str();
		int i_hash=s.length();
		size_t hash_perl=0;
		while(i_hash--)
			hash_perl=(hash_perl*33) + *sx++;
		return hash_perl + (hash_perl>>5);
}

size_t PerlJenkinsHashString::computeHash(string s) {
	const char* sx=s.c_str();
	size_t hash, i;
    for(hash = i = 0; i < s.length(); ++i)
    {
        hash += sx[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

size_t MurMurHashString::computeHash(string s) {
	const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;
	const int len=s.size();

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
	case 7: h ^= uint64_t(data2[6]) << 48;
	case 6: h ^= uint64_t(data2[5]) << 40;
	case 5: h ^= uint64_t(data2[4]) << 32;
	case 4: h ^= uint64_t(data2[3]) << 24;
	case 3: h ^= uint64_t(data2[2]) << 16;
	case 2: h ^= uint64_t(data2[1]) << 8;
	case 1: h ^= uint64_t(data2[0]);
	        h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

HashString* HashString::getHashStringFromConfig() {
	if(hashString==nullptr){
		switch (Config::getInstance()->getHashType()) {
		case HashType::STL_HASH:
			hashString = new STLHashString;
			break;
		case HashType::BOOST_HASH:
			hashString = new BOOSTHashString;
			break;
		case HashType::JAVA_HASH:
			hashString=new JavaHashString;
			break;
		case HashType::MUR_HASH:
			hashString=new MurMurHashString;
			break;
		case HashType::PERL_DJ:
			hashString=new PerlJenkinsHashString;
			break;
		case HashType::PERL_B:
			hashString=new PerlBernsteinHashString;
			break;

		default:
			hashString = new STLHashString;
			break;
		}
	}
	return hashString;
}
