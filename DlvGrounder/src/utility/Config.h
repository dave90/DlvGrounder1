/*
 * Config.h
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <cstring>

using namespace std;

static const char *TermTableTypeString[]={"BOOST","STL","MAP"};
enum TermTableType {
	BOOST=0, STL, MAP
};
static int N_TT=TermTableType::MAP;


static const char *HashTypeString[]={"STL_HASH" , "BOOST_HASH", "JAVA_HASH", "MUR_HASH,PERL_DJ","PERL_B"};
enum HashType {
	STL_HASH=0 , BOOST_HASH, JAVA_HASH, MUR_HASH,PERL_DJ,PERL_B
};
static int N_H=HashType::PERL_B;

static const char *IndexTypeString[]={"DEFAULT"};
enum IndexType {
	DEFAULT=0
};
static int N_I=IndexType::DEFAULT;

class Config {
public:
	static Config* getInstance();

	HashType getHashType() const;
	void setHashType(HashType hashType);
	void setHashType(string hashType);


	TermTableType getTermTableType() const;
	void setTermTableType(TermTableType termTableType);
	void setTermTableType(string termTableType);


	IndexType getIndexType() const;
	void setIndexType(IndexType indexType);
	void setIndexType(string indexType);

	bool isComponent() const {return component;}

	void setComponent(bool component) {	this->component = component;}

	bool isDependency() const {	return dependency;}

	void setDependency(bool dependency) {this->dependency = dependency;}

	bool isParser() const {	return parser;}

	void setParser(bool parser) {this->parser = parser;}

	bool isStatistic() const {	return statistic;}

	void setStatistic(bool statistic) {this->statistic = statistic;}

private:
	Config();
	static Config* config;
	TermTableType termTableType;
	HashType hashType;
	IndexType indexType;


	bool parser;
	bool dependency;
	bool component;
	bool statistic;

};

#endif /* CONFIG_H_ */
