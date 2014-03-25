/*
 * Instance.cpp
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#include "Instance.h"
#include "../utility/Config.h"

Instance::Instance(unsigned long predicate){
	this->predicate=predicate;
	switch(Config::getInstance()){
		default:
			indexAtom=0; //FIXME
			break;
	}
}

Instance::~Instance(){
	for(auto it = facts.begin(); it!= facts.end(); it++) delete(*it);
	for(auto it = nofacts.begin(); it!= nofacts.end(); it++) delete(*it);
	delete(indexAtom);
};
