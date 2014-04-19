/*
 * Config.cpp
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#include "Config.h"

Config* Config::config;

static string TermTableTypeString[]={ "BOOST", "STL", "MAP" };
static string HashTypeString[] ={ "STL_HASH", "BOOST_HASH", "JAVA_HASH", "MUR_HASH,PERL_DJ",
	"PERL_B" };
static string IndexTypeString[] ={ "DEFAULT" };

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

	 N_TT = TermTableType::MAP+1;
	 N_H = HashType::PERL_B+1;
	 N_I = IndexType::DEFAULT+1;

}

HashType Config::getHashType() const {
	return hashType;
}

void Config::setHashType(HashType hashType) {
	this->hashType = hashType;
}
void Config::setHashType(string hashType) {
	for(int i=0;i<N_H;i++){
		if(strcmp(HashTypeString[i].c_str(),hashType.c_str())==0)
			this->hashType=static_cast<HashType>(i);
	}
}

TermTableType Config::getTermTableType() const {
	return termTableType;
}

void Config::setTermTableType(TermTableType termTableType) {
	this->termTableType = termTableType;
}
void Config::setTermTableType(string termTableType) {
	for(int i=0;i<N_TT;i++){
		if(strcmp(TermTableTypeString[i].c_str(),termTableType.c_str())==0)
			this->termTableType=static_cast<TermTableType>(i);
	}
}


IndexType Config::getIndexType() const {
	return indexType;
}

void Config::setIndexType(IndexType indexType) {
	this->indexType = indexType;
}
void Config::setIndexType(string indexType) {
	for(int i=0;i<N_I;i++){
		if(strcmp(IndexTypeString[i].c_str(),indexType.c_str())==0)
			this->indexType=static_cast<IndexType>(i);
	}}
