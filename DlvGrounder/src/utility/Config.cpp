/*
 * Config.cpp
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#include "Config.h"

Config* Config::config;

Config* Config::getInstance() {
	if(config==0)
		config=new Config;

	return config;
}

Config::Config() {
	hashType=HashType::STL_HASH;
	termTableType=TermTableType::STL;
	indexType=IndexType::DEFAULT;
}

HashType Config::getHashType() const {
	return hashType;
}

void Config::setHashType(HashType hashType) {
	this->hashType = hashType;
}
void Config::setHashType(string hashType) {
	for(int i=0;i<N_H;i++){
		if(strcmp(HashTypeString[i],hashType.c_str())==0)
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
		if(strcmp(TermTableTypeString[i],termTableType.c_str())==0)
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
		if(strcmp(IndexTypeString[i],indexType.c_str())==0)
			this->indexType=static_cast<IndexType>(i);
	}}
