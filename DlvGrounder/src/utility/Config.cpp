/*
 * Config.cpp
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#include "Config.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "../table/IdsManager.h"

Config* Config::config;

static string TermTableTypeString[] = { "BOOST", "STL" };
static string HashTypeString[] = { "STL_HASH", "BOOST_HASH", "JAVA_HASH", "MUR_HASH","PERL_DJ", "PERL_B" };
static string IndexTypeString[] = { "DEFAULT" , "MULTIMAP", "MAP"};

Config* Config::getInstance() {
	if(config==0)
		config=new Config;

	return config;
}

Config::Config() {
	hashType=HashType::STL_HASH;
	termTableType=TermTableType::STL;
	indexType=IndexType::DEFAULT;
	component=false;
	parser=false;
	dependency=false;
	statistic=false;
	printRules=false;

	numberTermTableTypes = TermTableType::STL+1;
	numberHashType = HashType::PERL_B+1;
	numberIndexType = IndexType::MAP+1;
}

void Config::setHashType(string hashType) {
	for(int i=0;i<numberHashType;i++){
		if(strcmp(HashTypeString[i].c_str(),hashType.c_str())==0)
			this->hashType=static_cast<HashType>(i);
	}
}

void Config::setTermTableType(string termTableType) {
	for(int i=0;i<numberTermTableTypes;i++){
		if(strcmp(TermTableTypeString[i].c_str(),termTableType.c_str())==0)
			this->termTableType=static_cast<TermTableType>(i);
	}
}

void Config::setIndexType(string indexType) {
	for(int i=0;i<numberIndexType;i++){
		if(strcmp(IndexTypeString[i].c_str(),indexType.c_str())==0)
			this->indexType=static_cast<IndexType>(i);
	}
}

pair<unsigned int,bool> Config::getIndexingTerm(index_object predicate){
	if(this->indexingMap.count(predicate))
		return {indexingMap[predicate],true};
	return {0,false};
}

void Config::configureIndexingMap(){

	// Split the string indexingPreferences and
	// fill in the map with the indexing strategies for the specified

	if(indexingPreferences.compare("")!=0){
		stringstream stream(indexingPreferences);
		string segment;
		while(getline(stream, segment, ','))
		{
			vector<string> pair_pred_term;
			boost::split(pair_pred_term, segment, boost::is_any_of("="));
			pair_long_bool predIndex=IdsManager::getLongIndex(IdsManager::PREDICATE_ID_MANAGER,pair_pred_term[0]);
			if(predIndex.second){
				indexingMap.insert({predIndex.first,boost::lexical_cast<unsigned int>(pair_pred_term[1])});
			}
		}
	}

}
