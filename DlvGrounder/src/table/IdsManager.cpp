/*
 * IdsManager.cpp
 *
 *  Created on: 25/mar/2014
 *      Author: Jessica
 */

#include "IdsManager.h"

#include <iostream>

#include "../utility/Config.h"

#include <boost/lexical_cast.hpp>


HashString* hash_string_table::hash;

void IdManager::setHashType() {
	hash_string_table::hash = HashString::getHashStringFromConfig();
}

IdManager::IdManager() {
	counter = 0;

	setHashType();
}


pair_long_bool IdManager::insert(string &s) {

	auto its = hashStringId.left.find(s);
	if (its == hashStringId.left.end()) {
		pair_long_bool pairLong_bool(counter, false);
		hashStringId.left.insert({s,counter});
		counter++;
		return pairLong_bool;
	} else {
		return make_pair(its->second,true);
	}

}


pair_long_bool IdManager::insert(unsigned int &i) {
	auto its = hashIntId.left.find(i);
	if (its == hashIntId.left.end()) {
		pair_long_bool pairLong_bool(counter, false);
		hashIntId.left.insert({i,counter});
		counter++;
		return pairLong_bool;
	} else {
		return make_pair(its->second,true);
	}

}

index_object IdManager::getCollision() {

	index_object collision = 0;
	for (unsigned i = 0; i < hashStringId.bucket_count(); ++i) {
		if(hashStringId.bucket_size(i)>1)
				collision++;
	}
	for (unsigned i = 0; i < hashIntId.bucket_count(); ++i) {
		if(hashIntId.bucket_size(i)>1)
				collision++;
	}

	return collision;
}


vector<IdManager> IdsManager::idsManager;

IdsManager::IdsManager() {
}

IdsManager::~IdsManager() {
}



pair_long_bool IdsManager::getIndex(unsigned int i, string& s){
	// If the index i is greater that the size of idsManager, then just a new IdManager is added in IdsManager.
	while(i>=idsManager.size()){
		idsManager.push_back(IdManager());
	}
	return idsManager[i].insert(s);

}

pair_long_bool IdsManager::getIndex(unsigned int i, unsigned int &s){
	// If the index i is greater that the size of idsManager, then just a new IdManager is added in IdsManager.
	while(i>=idsManager.size()){
		idsManager.push_back(IdManager());
	}
	return idsManager[i].insert(s);

}

int IdsManager::getConflict(unsigned int i) {
	return idsManager[i].getCollision();
}

string IdsManager::getString(unsigned int idManager, index_object index) {
	if(idManager>=idsManager.size())
			return 0;
	return idsManager[idManager].findName(index);
}

long IdsManager::getLongIndex(unsigned int idManager, string name){
	if(idManager>=idsManager.size())
		return -1;
	return idsManager[idManager].findIndex(name);
}

string IdManager::findStringName(index_object &index) {
	return hashStringId.right.find(index)->second;
}

unsigned int IdManager::findIntName(index_object &index) {
	return hashIntId.right.find(index)->second;
}

string IdManager::findName(index_object &index) {
	auto it = hashStringId.right.find(index);
	if(it!=hashStringId.right.end())
		return it->second;
	return boost::lexical_cast<string>(findIntName(index));
}

long IdManager::findIndex(string& name) {
	if(hashStringId.left.find(name)!=hashStringId.left.end())
		return hashStringId.left.find(name)->second;
	return -1;
}


