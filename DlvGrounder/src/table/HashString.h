/*
 * HashString.h
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#ifndef HASHSTRING_H_
#define HASHSTRING_H_

#include <string>
#include <functional>

using namespace std;

class HashString {
public:
	HashString(){};
	virtual size_t computeHash(string s) = 0;
};

class STLHashString: public HashString{
public:
	STLHashString(){};
	virtual size_t computeHash(string s);
};

class BOOSTHashString: public HashString{
public:
	BOOSTHashString(){};
	virtual size_t computeHash(string s);

};

class JavaHashString: public HashString{
public:
	JavaHashString(){};
	virtual size_t computeHash(string s);

};

#endif /* HASHSTRING_H_ */