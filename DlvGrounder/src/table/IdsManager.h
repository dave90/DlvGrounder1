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
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/bimap/set_of.hpp>
#include <boost/lexical_cast.hpp>
#include "HashString.h"

#include "../utility/IndexDefinition.h"


using namespace std;

typedef pair<string, index_object> pair_string_id;
typedef pair<unsigned int, index_object> pair_const_id;

typedef pair<index_object, bool> pair_long_bool;

/**
 *  Hash of string ad index object used for bimap definition
 */
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


typedef boost::bimap< boost::bimaps::unordered_set_of<string,hash_string_table>, boost::bimaps::unordered_set_of<index_object> > hashStringMap;
typedef boost::bimap< boost::bimaps::unordered_set_of<unsigned int,hash_string_table>, boost::bimaps::unordered_set_of<index_object> > hashIntegerMap;

/**
*	Manages the assignment of an index for each string or integer
*	and store it in a bimap
*/
class BimapIdManager {
public:
	///Empty constructor, initialize counter and the hash respect the input configuration
	BimapIdManager(){counter = 0;setHashType();};

	///Insert the string if not exist and return the index
	pair_long_bool insert(string &s);

	///Insert the integer if not exist and return the index
	pair_long_bool insert(unsigned int& s);

	//Find the string with the given index
	string findStringName(index_object &index){
		return hashStringId.right.find(index)->second;
	}

	//Find the integer with the given index
	unsigned int findIntName(index_object &index){
		return hashIntId.right.find(index)->second;
	}

	//Find the String or Integer with the given index and return a string
	string findName(index_object& index){
		auto it = hashStringId.right.find(index);
		if(it!=hashStringId.right.end())
			return it->second;
		return boost::lexical_cast<string>(findIntName(index));
	}

	//Find the Index with the given name
	pair_long_bool findIndex(string& name){
		if(hashStringId.left.find(name)!=hashStringId.left.end())
			return {hashStringId.left.find(name)->second,true};
		return {0,false};
	}

	//return the number of collisions
	index_object getCollision();
private:
	/**
	 *  Bimap of string and index
	 */
	hashStringMap hashStringId;
	/**
	*  Bimap of integer and index
	*/
	hashIntegerMap hashIntId;

	/**
	 *  Counter for assign the index
	 */
	index_object counter;

	/// Set the hash to be used in according to input configuration
	void setHashType(){
		hash_string_table::hash = HashString::getHashStringFromConfig();
	}

};


/**
*	Set of IdManager, for manage multiple IdManager
*	For each IdManager create an static final ID
*
*	The class is static
*/
class IdsManager {
public:
	/// Search in the idManager with specific ID the index of a string, if is not present insert the string in a IdManage
	static pair_long_bool getIndex(unsigned int idManager, string& s);
	/// Search in the idManager with specific ID the index of a integer, if is not present insert the string in a IdManage
	static pair_long_bool getIndex(unsigned int idManager, unsigned int& integer);

	/// Given an index and the ID of IdManager, returns the corresponding string
	static string getString(unsigned int idManager, index_object index);
	/// Given a string and the ID of IdManager, returns the corresponding index and true if exist the string, else false
	static pair_long_bool getLongIndex(unsigned int idManager, string name);

	/// Given an index and the ID of IdManager return the string stripped in the IdManager with the ID.
	/// The string is a substring of original string, start to index 0 to the first character '*'
	static string getStringStrip(unsigned int idManager, index_object index){string s=getString(idManager,index);return s.substr(0, s.find("*"));};

	/// Return the number of conflict
	static int getConflict(unsigned int i){
		return idsManager[i].getCollision();
	}

	/// The id of a IdManager that contain the term
	static const int TERM_ID_MANAGER=0;
	/// The id of a IdManager that contain the predicate
	static const int PREDICATE_ID_MANAGER=1;
	virtual ~IdsManager(){};
private:
	///Private constructor
	IdsManager(){};

	/**
	 *  A vector of IdManager, the index of each IdManager is a position of that vector
	 */
	static vector<BimapIdManager> idsManager;
};

#endif /* IDSMANAGER_H_ */
