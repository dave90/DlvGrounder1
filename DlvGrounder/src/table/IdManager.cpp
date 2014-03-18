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
	case HashType::MUR_HASH:
		hash_string_table::hash=new MurMurHashString;
		break;
	case HashType::BOOST_RANGE_HASH:
		hash_string_table::hash=new BOOSTHashRange;
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
	auto its = hashId.find(s);
	if (its == hashId.end()) {
		pair_long_bool pairLong_bool(counter, false);
		pString_Id.second = counter;
		counter++;
		hashId.insert(pString_Id);
		return pairLong_bool;
	} else {
		return make_pair(its->second,true);
	}

}

unsigned long IdManager::getCollision() {

	unsigned long collision = 0;
	for (unsigned i = 0; i < hashId.bucket_count(); ++i) {
		if(hashId.bucket_size(i)>1)
				collision++;
	}

	return collision;
}


