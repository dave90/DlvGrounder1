/*
 * Config.h
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#ifndef CONFIG_H_
#define CONFIG_H_

static const char *TermTableTypeString[]={"BOOST","STL","MAP"};
enum TermTableType {
	BOOST, STL, MAP
};


static const char *HashTypeString[]={"STL_HASH" , "BOOST_HASH", "JAVA_HASH", "MUR_HASH,PERL_DJ","PERL_B"};
enum HashType {
	STL_HASH , BOOST_HASH, JAVA_HASH, MUR_HASH,PERL_DJ,PERL_B
};

static const char *IndexTypeString[]={"DEFAULT"};
enum IndexType {
	DEFAULT
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


private:
	Config();
	static Config* config;
	TermTableType termTableType;
	HashType hashType;
	IndexType indexType;

};

#endif /* CONFIG_H_ */
