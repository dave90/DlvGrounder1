/*
 * IdManager.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Davide
 */

#include "IdManager.h"
#include <iostream>
#include "JavaHash.h"
#include "BoostHash.h"
#include "STLHash.h"

void IdManager::setHashType() {
	JavaHash java;
	BoostHash boost;
	STLHash stl;
//	switch (hashType) {
//	case 0:
//		hash_string::h = &stl;
//		break;
//	case 1:
//		hash_string::h = &boost;
//		break;
//	case 2:
//		hash_string::h = &java;
//		break;
//	default:
		hash_string::ptr_hash = &stl;
//		break;
//	}
}

IdManager::IdManager() {
	counter = 0;
	hashType = HashType::STL_HASH;
	setHashType();
}
IdManager::IdManager(HashType t) {
	counter = 0;
	hashType = t;
	setHashType();
}

pair_long_bool IdManager::insert(string s) {

	pair_string_id pString_Id(s, 0);
	auto its = hashId.equal_range(pString_Id);
	if (its.first == its.second) {
		pair_long_bool pairLong_bool(counter, false);
		pString_Id.second = counter;
		counter++;
		hashId.insert(pString_Id);
		return pairLong_bool;
	} else {
		for (auto it = its.first; it != its.second; ++it) {
			if (strcmp(s.c_str(), it->first.c_str()) == 0) {
				pair_long_bool pairLong_bool(it->second, true);
				return pairLong_bool;
			}
		}
	}

}

unsigned long IdManager::getCollision() {

	unsigned long collision = 0;

	for (unsigned i = 0; i < hashId.bucket_count(); ++i) {
		for (auto it = hashId.begin(i); it != hashId.end(i); ++it)
			if (it != hashId.begin(i))
				collision++;

	}

	return collision;
}
