/*
 * Config.h
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#ifndef CONFIG_H_
#define CONFIG_H_

enum TermTableType {
	BOOST, STL, MAP
};

enum HashType {
	STL_HASH , BOOST_HASH, JAVA_HASH, MUR_HASH,PERL_DJ,PERL_B
};

enum IndexType {
	DEFAULT
};

class Config {
public:
	static Config* getInstance();
	HashType getHashType() const;
	void setHashType(HashType hashType);
	TermTableType getTermTableType() const;
	void setTermTableType(TermTableType termTableType);
	IndexType getIndexType() const;
	void setIndexType(IndexType indexType);

private:
	Config();
	static Config* config;
	TermTableType termTableType;
	HashType hashType;
	IndexType indexType;

};

#endif /* CONFIG_H_ */
