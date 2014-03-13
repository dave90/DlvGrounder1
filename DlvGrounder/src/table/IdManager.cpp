/*
 * IdManager.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Davide
 */

#include "IdManager.h"
#include <iostream>

#include "../utility/Config.h"


HashString* hash_string_table::hash;

void IdManager::setHashType() {
	cout<<"Set hash to "<<Config::getInstance()->getHashType()<<endl;
	switch (Config::getInstance()->getHashType()) {
	case HashType::STL_HASH:
		hash_string_table::hash = new STLHashString;
		break;
	case HashType::BOOST_HASH:
		hash_string_table::hash = new BOOSTHashString;
		break;
	case HashType::JAVA_HASH:
		hash_string_table::hash=new JavaHashString;
		break;
	default:
		hash_string_table::hash = new STLHashString;
		break;
	}
}

IdManager::IdManager() {
	counter = 0;

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
