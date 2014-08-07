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

/// Different types of hash functions implemented
enum HashType {
	STL_HASH = 0, BOOST_HASH, JAVA_HASH, MUR_HASH, PERL_DJ, PERL_B
};

/// Different types of term table implemented
enum TermTableType {
	BOOST = 0, STL
};

/// Different types of indexing strategies implemented
enum IndexType {
	DEFAULT = 0, MULTIMAP, MAP
};

/**
 * This class manages the configuration set by the user providing command-line arguments.
 * It is implemented according to GOF Singleton pattern.
 */

class Config {
public:
	/// This static method returns the unique instance of Config
	static Config* getInstance();

	///Getter for the hash function type
	HashType getHashType() const {return this->hashType;}
	///Setter for the hash function type
	///@param hashType A value of type HashType defining the hash function
	void setHashType(HashType hashType) {this->hashType = hashType;}
	///Setter for the hash function type
	///@param hashType A string defining the hash function type
	void setHashType(string hashType);

	///Getter for the term table type
	TermTableType getTermTableType() const {return this->termTableType;}
	///Setter for the term table type
	///@param termTableType A value of type TermTableType defining the term table type @see TermTableType
	void setTermTableType(TermTableType termTableType) {this->termTableType = termTableType;}
	///Setter for the term table type
	///@param termTableType A string defining the term table type
	void setTermTableType(string termTableType);

	///Getter for the indexing strategy type
	IndexType getIndexType() const {return this->indexType;}
	///Setter for the indexing strategy type
	///@param indexType A value of type IndexingStrategyType defining the indexing strategy type @see IndexingStrategyType
	void setIndexType(IndexType indexType) {this->indexType=indexType;}
	///Setter for the indexing strategy type
	///@param indexType A string defining the indexing strategy type
	void setIndexType(string indexType);

	///This method returns if the component graph has to be printed
	bool isComponent() const {return component;}
	///This method sets if the component graph has to be printed
	void setComponent(bool component) {this->component = component;}

	///This method returns if the dependency graph has to be printed
	bool isDependency() const {return dependency;}
	///This method sets if the dependency graph has to be printed
	void setDependency(bool dependency) {this->dependency = dependency;}

	///This method returns if the outcome of the parsing has to be printed
	bool isParser() const {return parser;}
	///This method sets if the outcome of the parsing has to be printed
	void setParser(bool parser) {this->parser = parser;}

	///This method returns if the statistics about the program have to be printed
	bool isStatistic() const {return statistic;}
	///This method sets if the statistics about the program have to be printed
	void setStatistic(bool statistic) {this->statistic = statistic;}

	///Getter for the name of the file in which the dependency graph as to be printed
	string getFileGraph() const {return fileGraph;}
	///Setter for the name of the file in which the dependency graph as to be printed
	void setFileGraph(string f) {this->fileGraph = f;}

	///This method returns if the rules composing the program have to be printed
	bool isPrintRules() const {return printRules;}
	///This method sets if the rules composing the program have to be printed
	void setPrintRules(bool pr) {this->printRules = pr;}

	///Getter for the indexing preferences
	const string& getIndexingPreferences() const {return indexingPreferences;}
	///Setter for the indexing preferences
	void setIndexingPreferences(const string& indexingPreferences) {this->indexingPreferences = indexingPreferences;}

	///This method configures the indexing strategies for each predicate provided by the user
	void configureIndexingMap();

	///This method, given a predicate, returns the corresponding position of the indexing term
	pair<unsigned int,bool> getIndexingTerm(index_object predicate);

private:
	///Default Constructor
	Config();

	///The single and unique static instance of Config
	static Config* config;

	///The hash function type
	TermTableType termTableType;
	///The term table type
	HashType hashType;
	///The indexing strategy type
	IndexType indexType;

	///Whether the parsing outcome has to be printed
	bool parser;
	///Whether the dependency graph has to be printed
	bool dependency;
	///Whether the component graph has to be printed
	bool component;
	///Whether the statistics have to be printed
	bool statistic;
	///Whether the rules have to be printed
	bool printRules;

	///The string whit the indexing preference
	string indexingPreferences;
	///The map containing the mapping between a predicate and the position of its indexing term
	unordered_map<index_object, unsigned int> indexingMap;

	///The name of the output file for the dependency graph
	string fileGraph;

	///The number of different term table types
	int numberTermTableTypes;
	///The number of different hash function types
	int numberHashType;
	///The number of different indexing strategy types
	int numberIndexType;

};

#endif /* CONFIG_H_ */
