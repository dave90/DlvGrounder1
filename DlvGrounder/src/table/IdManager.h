/*
 * IdManager.h
 *
 *  Created on: 07/mar/2014
 *      Author: Davide
 */

#ifndef IDMANAGER_H_
#define IDMANAGER_H_

#include <unordered_set>
#include <string>
#include <cstring>
#include <stdlib.h>

#include "HashString.h"

/*
 * This class manage the id for the string
 */

using namespace std;

typedef pair<string, unsigned long> pair_string_id;
typedef pair<unsigned long, bool> pair_long_bool;


/*
 *  The hash for the string
 */
struct hash_string_table {
	static HashString* hash;
	size_t operator()(const pair_string_id &p) const {
		return hash->computeHash(p.first);
	}
};


struct equalString {
	bool operator()(const pair_string_id &p1, const pair_string_id &p2) const {
		return strcmp(p1.first.c_str(), p2.first.c_str()) == 0;
	}
};

enum HashType { STL_HASH=0,BOOST_HASH,JAVA_HASH };

class IdManager {
public:
	IdManager();
	IdManager(HashType t);
	//return an index and if the string exist
	pair_long_bool insert(string s);
	//return the number of collision
	unsigned long getCollision();
private:
	unordered_multiset<pair_string_id, hash_string_table, equalString> hashId;
	unsigned long counter;
	HashType hashType;

	void setHashType();
};

#endif /* IDMANAGER_H_ */
