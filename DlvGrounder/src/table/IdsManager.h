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
//#include <unordered_map>
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/bimap/set_of.hpp>

#include "HashString.h"

using namespace std;

typedef pair<string, unsigned long> pair_string_id;
typedef pair<unsigned int, unsigned long> pair_const_id;

typedef pair<unsigned long, bool> pair_long_bool;


/*
 *  The hash for the string
 */
//template <class T>
//class hash_string_table {
//private:
//	static HashString* hash;
//public:
//	size_t operator()(string p) const {
//		return hash->computeHash(p);
//	}
//};

struct hash_string_table {
	static HashString* hash;
	size_t operator()(string p) const {
		return hash->computeHash(p);
	}
	bool operator()(const pair_string_id &p1, const pair_string_id &p2) const {
		return strcmp(p1.first.c_str(), p2.first.c_str()) == 0;
	}
	size_t operator()(unsigned int p) const {
		return p;
	}
	bool operator()(const pair_const_id &p1, const pair_const_id &p2) const {
		return p1.first == p2.first;
	}
};


typedef boost::bimap< boost::bimaps::unordered_set_of<string,hash_string_table>, boost::bimaps::set_of<unsigned long> > hashStringMap;
typedef boost::bimap< boost::bimaps::unordered_set_of<unsigned int,hash_string_table>, boost::bimaps::set_of<unsigned long> > hashIntegerMap;


class IdManager {
public:
	IdManager();
	//return an index and if the string exist
	pair_long_bool insert(string &s);
	//return an index and if the integer exist
	pair_long_bool insert(unsigned int& s);
	//Find the String with the given index
	string findStringName(unsigned long &index);
	//Find the Integer with the given index
	unsigned int findIntName(unsigned long &index);
	//Find the String or Integer with the given index and return the string of integer
	string findName(unsigned long& index);
	//Find the Index with the given name
	long findIndex(string& name);
	//return the number of collisions
	unsigned long getCollision();
private:
	hashStringMap hashStringId;
	hashIntegerMap hashIntId;

//	unordered_map<string, unsigned long,hash_string_table> hashId;
	unsigned long counter;

	void setHashType();
};

class IdsManager {
public:
	virtual ~IdsManager();
	static pair_long_bool getIndex(unsigned int idManager, string& s);
	static pair_long_bool getIndex(unsigned int idManager, unsigned int& s);

	// Given an index and the IdManager type, returns the corresponding string
	static string getString(unsigned int idManager, unsigned long index);
	static long getLongIndex(unsigned int idManager, string name);
	static string getStringStrip(unsigned int idManager, unsigned long index){string s=getString(idManager,index);return s.substr(0, s.find("*"));};
	static int getConflict(unsigned int i);
	static const int TERM_ID_MANAGER=0;
	static const int PREDICATE_ID_MANAGER=1;
private:
	IdsManager();
	static vector<IdManager> idsManager;
};

#endif /* IDSMANAGER_H_ */
