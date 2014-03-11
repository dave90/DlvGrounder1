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

/*
 * This class manage the id for the string
 */

using namespace std;

typedef pair<string, unsigned long> pair_string_id;
typedef pair<unsigned long, bool> pair_long_bool;

/*
 *  The hash for the string
 */
struct stl_hash_string {
	size_t operator()(const pair_string_id &p) const {
		hash<string> hash_fn;
		size_t str_hash = hash_fn(p.first);
		return str_hash;
	}

};

struct stl_hash_string_java {
		size_t operator()(const pair_string_id &p) const {
			const char* sx=p.first;
			int length=p.first.length();
			int hash=0;
			for (int i = 0; i < length; ++i) {
				hash+=sx[0]*pow(31.0,length-(i+1));
			}
			return hash;
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
	//return the number of collision
	unsigned long getCollision();
private:
	unordered_multiset<pair_string_id, stl_hash_string_java, equalString> hashId;
	unsigned long counter;
};

#endif /* IDMANAGER_H_ */
