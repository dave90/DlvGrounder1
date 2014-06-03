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
	hash_string_table::hash = HashString::getHashStringFromConfig();
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
	return idsManager[idManager].findName(index);
}

long IdsManager::getLongIndex(unsigned int idManager, string name){
	if(idManager>=idsManager.size())
		return -1;
	return idsManager[idManager].findIndex(name);
}

string IdManager::findName(unsigned long index) {
	return hashId.right.find(index)->second;
}

long IdManager::findIndex(string name) {
	if(hashId.left.find(name)!=hashId.left.end())
		return hashId.left.find(name)->second;
	return -1;
}


