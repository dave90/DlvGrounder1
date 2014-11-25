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
#include <stdlib.h>
#include <ctime>

using namespace std;

/**
 *  Abstract class that compute the hash of string
 *  The class is a singleton
 */
class HashString {
public:
	HashString(){};
	virtual ~HashString(){delete hashString;}
	///Compute the hash of a string
	virtual size_t computeHash(string s) = 0;

	/// Return an hashString according the configuration
	static HashString* getHashStringFromConfig();
private:
	static HashString *hashString;
};

/**
 *  Compute the hash  with stl method
 */
class STLHashString: public HashString{
public:
	STLHashString(){};
	virtual size_t computeHash(string s);
};

/**
 *  Compute the hash  with boost method
 */
class BOOSTHashString: public HashString{
public:
	BOOSTHashString(){};
	virtual size_t computeHash(string s);

};

/**
 *  Compute the hash  with java method
 */
class JavaHashString: public HashString{
public:
	JavaHashString(){};
	virtual size_t computeHash(string s);

};

/**
 *  Compute the hash  with perl Bernstein method
 */
class PerlBernsteinHashString: public HashString{
public:
	PerlBernsteinHashString(){};
	virtual size_t computeHash(string s);

};

/**
 *  Compute the hash  with perl Jenkins method
 */
class PerlJenkinsHashString: public HashString{
public:
	PerlJenkinsHashString(){};
	virtual size_t computeHash(string s);

};

/**
 *  Compute the hash  with murmur method
 */
class MurMurHashString: public HashString{
public:
	MurMurHashString(){srand (time(NULL));seed=rand();};
	virtual size_t computeHash(string s);
private:
	int seed;
};

#endif /* HASHSTRING_H_ */
