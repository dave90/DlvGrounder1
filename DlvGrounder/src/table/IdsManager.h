/*
 * IdsManager.h
 *
 *  Created on: 25/mar/2014
 *      Author: Jessica
 */

#ifndef IDSMANAGER_H_
#define IDSMANAGER_H_

/*
 * This class manage the id for the string
 */

#include <vector>
#include <cstring>
#include <unordered_map>

#include "HashString.h"


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

class IdsManager {
public:
	virtual ~IdsManager();
	static pair_long_bool getIndex(unsigned int i, string s);
	static int getConflict(unsigned int i);
	static const int TERM_ID_MANAGER=0;
	static const int PREDICATE_ID_MANAGER=1;
	static const int ATOM_ID_MANAGER=2;
private:
	IdsManager();
	static vector<IdManager> idsManager;
};

#endif /* IDSMANAGER_H_ */
