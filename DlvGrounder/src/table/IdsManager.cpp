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

/****************** IDMANAGER *********************/

/****************** ABSTRACT IDMANAGER *********************/

HashString* hash_string_table::hash;

void AbstractIdManager::setHashType() {
	hash_string_table::hash = HashString::getHashStringFromConfig();
}

AbstractIdManager::AbstractIdManager() {
	counter = 0;
	setHashType();
}

/****************** END ABSTRACT IDMANAGER *********************/

/****************** BIMAP IDMANAGER *********************/
pair_long_bool BimapIdManager::insert(string &s) {

	pair_long_bool pairLong_bool(counter, false);
	auto its = hashStringId.left.insert({s,counter});
	if (its.second) {
		counter++;
		return pairLong_bool;
	} else {
		return {its.first->second,true};
	}
}

pair_long_bool BimapIdManager::insert(unsigned int &i) {
	pair_long_bool pairLong_bool(counter, false);
	auto its = hashIntId.left.insert({i,counter});
	if (its.second) {
		counter++;
		return pairLong_bool;
	} else {
		return {its.first->second,true};
	}
}

index_object BimapIdManager::getCollision() {
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

string BimapIdManager::findStringName(index_object &index) {
	return hashStringId.right.find(index)->second;
}

unsigned int BimapIdManager::findIntName(index_object &index) {
	return hashIntId.right.find(index)->second;
}

string BimapIdManager::findName(index_object &index) {
	auto it = hashStringId.right.find(index);
	if(it!=hashStringId.right.end())
		return it->second;
	return boost::lexical_cast<string>(findIntName(index));
}

pair_long_bool BimapIdManager::findIndex(string& name) {
	if(hashStringId.left.find(name)!=hashStringId.left.end())
		return {hashStringId.left.find(name)->second,true};
	return {0,false};
}

/****************** END BIMAP IDMANAGER *********************/

/****************** TWOMAPS IDMANAGER *********************/

pair_long_bool TwoMapsIdManager::insert(string &s) {
	pair_long_bool pairLong_bool(counter, false);
	auto inserted=hashStringIdStringKey.insert({s,counter});
	if(inserted.second){
		hashStringIdIndexObjectKey.insert({&inserted.first->second,&(inserted.first->first)});
		counter++;
	}
	else{
		pairLong_bool.first=inserted.first->second;
		pairLong_bool.second=true;
	}
	return pairLong_bool;
}


pair_long_bool TwoMapsIdManager::insert(unsigned int &i) {
	pair_long_bool pairLong_bool(counter, false);
	auto inserted=hashIntegerIdIntegerKey.insert({i,counter});
	if(inserted.second){
		hashIntegerIdIndexObjectKey.insert({&inserted.first->second,&(inserted.first->first)});
		counter++;
	}
	else{
		pairLong_bool.first=inserted.first->second;
		pairLong_bool.second=true;
	}
	return pairLong_bool;

}

index_object TwoMapsIdManager::getCollision() {
	unsigned int collision = 0;
	for (unsigned i = 0; i < hashStringIdStringKey.bucket_count(); ++i) {
		if(hashStringIdStringKey.bucket_size(i)>1)
				collision++;
	}
	for (unsigned i = 0; i < hashStringIdIndexObjectKey.bucket_count(); ++i) {
			if(hashStringIdIndexObjectKey.bucket_size(i)>1)
				collision++;
	}
	for (unsigned i = 0; i < hashIntegerIdIntegerKey.bucket_count(); ++i) {
		if(hashIntegerIdIntegerKey.bucket_size(i)>1)
				collision++;
	}
	for (unsigned i = 0; i < hashIntegerIdIndexObjectKey.bucket_count(); ++i) {
		if(hashIntegerIdIndexObjectKey.bucket_size(i)>1)
				collision++;
	}

	return collision;
}

string TwoMapsIdManager::findStringName(index_object &index) {
	auto it = hashStringIdIndexObjectKey.find(&index);
	if(it!=hashStringIdIndexObjectKey.end()){
		return *(it->second);
	}
	return "";
}

unsigned int TwoMapsIdManager::findIntName(index_object &index) {
	auto it = hashIntegerIdIndexObjectKey.find(&index);
	if(it!=hashIntegerIdIndexObjectKey.end()){
		return *(it->second);
	}
	return 0;
}

string TwoMapsIdManager::findName(index_object &index) {
	auto it = hashStringIdIndexObjectKey.find(&index);
	if(it!=hashStringIdIndexObjectKey.end()){
		return *it->second;
	}
	return boost::lexical_cast<string>(findIntName(index));
}

pair_long_bool TwoMapsIdManager::findIndex(string& name) {
	auto it = hashStringIdStringKey.find(name);
	if(it!=hashStringIdStringKey.end())
		return {it->second,true};
	return {0,false};
}

/****************** END TWOMAPS IDMANAGER *********************/

/****************** ONEMAP IDMANAGER *********************/

pair_long_bool OneMapIdManager::insert(string &s) {
	pair_long_bool pairLong_bool(counter, false);
	auto inserted=hashStringIdStringKey.insert({s,counter});
	if(inserted.second)
		counter++;
	else{
		pairLong_bool.first=inserted.first->second;
		pairLong_bool.second=true;
	}
	return pairLong_bool;
}


pair_long_bool OneMapIdManager::insert(unsigned int &i) {
	pair_long_bool pairLong_bool(counter, false);
	auto inserted=hashIntegerIdIntegerKey.insert({i,counter});
	if(inserted.second)
		counter++;
	else{
		pairLong_bool.first=inserted.first->second;
		pairLong_bool.second=true;
	}
	return pairLong_bool;

}

index_object OneMapIdManager::getCollision() {
	unsigned int collision = 0;
	for (unsigned i = 0; i < hashStringIdStringKey.bucket_count(); ++i) {
		if(hashStringIdStringKey.bucket_size(i)>1)
				collision++;
	}
	for (unsigned i = 0; i < hashIntegerIdIntegerKey.bucket_count(); ++i) {
		if(hashIntegerIdIntegerKey.bucket_size(i)>1)
				collision++;
	}

	return collision;
}

string OneMapIdManager::findStringName(index_object &index) {
	return "";
}

unsigned int OneMapIdManager::findIntName(index_object &index) {
	return 0;
}

string OneMapIdManager::findName(index_object &index) {
	return "";
}

pair_long_bool OneMapIdManager::findIndex(string& name) {
	auto it = hashStringIdStringKey.find(name);
	if(it!=hashStringIdStringKey.end())
		return {it->second,true};
	return {0,false};
}

/****************** END ONEMAP IDMANAGER *********************/

/****************** END IDMANAGER *********************/

/****************** IDSMANAGER *********************/

vector<IdManager> IdsManager::idsManager;

IdsManager::IdsManager() {}

IdsManager::~IdsManager() {}

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

pair_long_bool IdsManager::getLongIndex(unsigned int idManager, string name){
	if(idManager>=idsManager.size())
		return {0,false};
	return idsManager[idManager].findIndex(name);
}

/****************** END IDSMANAGER *********************/


