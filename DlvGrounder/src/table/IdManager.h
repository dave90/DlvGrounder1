/*
 * IdManager.h
 *
 *  Created on: 07/mar/2014
 *      Author: Davide
 */

#ifndef IDMANAGER_H_
#define IDMANAGER_H_

#include <unordered_set>
#include <unordered_map>
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
	size_t operator()(string p) const {
		return hash->computeHash(p);
	}
};


struct equalString {
	bool operator()(const pair_string_id &p1, const pair_string_id &p2) const {
		return strcmp(p1.first.c_str(), p2.first.c_str()) == 0;
	}
};


class IdManager {
public:
	IdManager();
	//return an index and if the string exist
	pair_long_bool insert(string s);
	//insert with pre hash
	pair_long_bool insert(size_t hash,string s);
	//return the number of collision
	unsigned long getCollision();
private:
	unordered_map<string, unsigned long,hash_string_table> hashId;
	unsigned long counter;

	void setHashType();
};

#endif /* IDMANAGER_H_ */
