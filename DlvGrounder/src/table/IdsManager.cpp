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



/****************** BIMAP IDMANAGER *********************/

HashString* hash_string_table::hash;


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




/****************** END BIMAP IDMANAGER *********************/



/****************** IDSMANAGER *********************/

vector<BimapIdManager> IdsManager::idsManager;


pair_long_bool IdsManager::getIndex(unsigned int i, string& s){
	// If the index i is greater that the size of idsManager, then just a new IdManager is added in IdsManager.
	while(i>=idsManager.size()){
		idsManager.push_back(BimapIdManager());
	}
	return idsManager[i].insert(s);

}

pair_long_bool IdsManager::getIndex(unsigned int i, unsigned int &s){
	// If the index i is greater that the size of idsManager, then just a new IdManager is added in IdsManager.
	while(i>=idsManager.size()){
		idsManager.push_back(BimapIdManager());
	}
	return idsManager[i].insert(s);

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


