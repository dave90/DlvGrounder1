/*
 * IdsManager.cpp
 *
 *  Created on: 25/mar/2014
 *      Author: Jessica
 */

#include "IdsManager.h"

vector<IdManager> IdsManager::idsManager;

IdsManager::IdsManager() {
}

IdsManager::~IdsManager() {
}



pair_long_bool IdsManager::getIndex(unsigned int i, string s){
	// If the index i is greater that the size of idsManager, then just a new IdManager is added in IdsManager.
	if(i>=idsManager.size()){
		idsManager.push_back(IdManager());
		i=idsManager.size()-1;
	}
	return idsManager[i].insert(s);

}
