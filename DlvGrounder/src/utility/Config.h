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
#include <unordered_map>

#include "IndexDefinition.h"


using namespace std;

enum TermTableType {
	BOOST = 0, STL, MAP
};

enum HashType {
	STL_HASH = 0, BOOST_HASH, JAVA_HASH, MUR_HASH, PERL_DJ, PERL_B
};

enum IndexType {
	DEFAULT = 0
};

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

	void configureIndexingMap();
	unsigned int getIndexingTerm(index_object predicate);

	bool isComponent() const {
		return component;
	}

	void setComponent(bool component) {
		this->component = component;
	}

	bool isDependency() const {
		return dependency;
	}

	void setDependency(bool dependency) {
		this->dependency = dependency;
	}

	bool isParser() const {
		return parser;
	}

	void setParser(bool parser) {
		this->parser = parser;
	}

	bool isStatistic() const {
		return statistic;
	}

	void setStatistic(bool statistic) {
		this->statistic = statistic;
	}

	string getFileGraph() const {
		return fileGraph;
	}

	void setFileGraph(string f) {
		this->fileGraph = f;
	}

	bool isPrintRules() const {
		return printRules;
	}

	void setPrintRules(bool pr) {
		this->printRules = pr;
	}

	const string& getIndexingPreferences() const {
		return indexingPreferences;
	}

	void setIndexingPreferences(const string& indexingPreferences) {
		this->indexingPreferences = indexingPreferences;
	}

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
	bool printRules;

	string indexingPreferences;
	unordered_map<index_object, index_object> indexingMap;

	string fileGraph;


	int N_TT ;
	int N_H ;
	int N_I;

};

#endif /* CONFIG_H_ */
