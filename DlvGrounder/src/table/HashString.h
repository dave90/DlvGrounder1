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

class HashString {
public:
	HashString(){};
	~HashString(){delete hashString;}
	virtual size_t computeHash(string s) = 0;

	/// Return an hashString according the configuration
	static HashString* getHashStringFromConfig();
private:
	static HashString *hashString;
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

class PerlBernsteinHashString: public HashString{
public:
	PerlBernsteinHashString(){};
	virtual size_t computeHash(string s);

};

class PerlJenkinsHashString: public HashString{
public:
	PerlJenkinsHashString(){};
	virtual size_t computeHash(string s);

};

class BOOSTHashRange: public HashString{
public:
	BOOSTHashRange(){};
	virtual size_t computeHash(string s);

};

class MurMurHashString: public HashString{
public:
	MurMurHashString(){srand (time(NULL));seed=rand();};
	virtual size_t computeHash(string s);
private:
	int seed;
};

#endif /* HASHSTRING_H_ */
