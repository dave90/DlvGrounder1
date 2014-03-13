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
}

HashType Config::getHashType() const {
	return hashType;
}

void Config::setHashType(HashType hashType) {
	this->hashType = hashType;
}

TermTableType Config::getTermTableType() const {
	return termTableType;
}

void Config::setTermTableType(TermTableType termTableType) {
	this->termTableType = termTableType;
}
