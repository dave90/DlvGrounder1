/*
 * IdManager.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Davide
 */

#include "IdManager.h"
#include <iostream>

IdManager::IdManager() {
	counter=0;
}

pair_long_bool IdManager::insert(string s) {

//	for (unsigned i=0; i<hashId.bucket_count(); ++i) {
//	    std::cout << "bucket #" << i << " contains:";
//	    for (auto it = hashId.begin(i); it!=hashId.end(i); ++it)
//	      std::cout << " " << (*it).first<<"-"<<(*it).second;
//	    std::cout << "\n";
//	  }

	pair_string_id pString_Id(s, 0);
	auto its = hashId.equal_range(pString_Id);
	if (its.first == its.second) {
		pair_long_bool pairLong_bool(counter,false);
		pString_Id.second=counter;
		counter++;
		hashId.insert(pString_Id);
		return pairLong_bool;
	} else {
		for (auto it = its.first; it != its.second; ++it) {
			if (strcmp(s.c_str(), it->first.c_str()) == 0) {
				pair_long_bool pairLong_bool(it->second,true);
				return pairLong_bool;
			}
		}
	}

}
