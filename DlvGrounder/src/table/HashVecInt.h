/*
 * HashVecInt.h
 *
 *  Created on: Jun 17, 2014
 *      Author: david
 */

#ifndef HASHVECINT_H_
#define HASHVECINT_H_

#include <vector>

#include <boost/functional/hash.hpp>

#include "../utility/IndexDefinition.h"

using namespace std;


class HashVecInt {
public:
	HashVecInt(){};
	virtual size_t computeHash(const vector<index_object>& values)=0;
	virtual size_t computeHashSize_T(const vector<size_t>& values)=0;

	virtual ~HashVecInt(){};
	/// Return an HashVecInt according the configuration
	static HashVecInt* getHashVecIntFromConfig();
private:
	static HashVecInt *hashInt;
};

class JavaHashVecInt : public HashVecInt{
public:
	JavaHashVecInt(){}
	inline size_t computeHash(const vector<index_object> &values){
		int length=values.size();
		size_t hash=0;
		for (int i = 0; i < length; i++) {
			hash+=values[i]*pow(31.0,length-(i));
		}

		return hash;
	}
	inline size_t computeHashSize_T(const vector<size_t> &values){
		int length=values.size();
		size_t hash=0;
		for (int i = 0; i < length; i++) {
			hash+=values[i]*pow(31.0,length-(i));
		}

		return hash;
	}

};

class BoostCombineHashVecInt : public HashVecInt{
public:
	inline size_t computeHash(const vector<index_object> & values){
		return boost::hash_range(values.begin(),values.end());
	}
	inline size_t computeHashSize_T(const vector<size_t> & values){
		return boost::hash_range(values.begin(),values.end());
	}

};

#endif /* HASHVECINT_H_ */
