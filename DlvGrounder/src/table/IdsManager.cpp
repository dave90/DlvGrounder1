/*
 * IdsManager.cpp
 *
 *  Created on: 25/mar/2014
 *      Author: Jessica
 */

#include "IdsManager.h"

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
	case HashType::PERL_DJ:
		hash_string_table::hash=new PerlJenkinsHashString;
		break;
	case HashType::PERL_B:
		hash_string_table::hash=new PerlBernsteinHashString;
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
	auto its = hashId.left.find(s);
	if (its == hashId.left.end()) {
		pair_long_bool pairLong_bool(counter, false);
		hashId.left.insert({s,counter});
		counter++;
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


vector<IdManager> IdsManager::idsManager;

IdsManager::IdsManager() {
}

IdsManager::~IdsManager() {
}



pair_long_bool IdsManager::getIndex(unsigned int i, string s){
	// If the index i is greater that the size of idsManager, then just a new IdManager is added in IdsManager.
	while(i>=idsManager.size()){
		idsManager.push_back(IdManager());
	}
	return idsManager[i].insert(s);

}

int IdsManager::getConflict(unsigned int i) {
	return idsManager[i].getCollision();
}

string IdsManager::getString(unsigned int idManager, unsigned long index) {
	if(idManager>=idsManager.size())
			return 0;
	return idsManager[idManager].find(index);
}

string IdManager::find(unsigned long index) {
	return hashId.right.find(index)->second;
}
