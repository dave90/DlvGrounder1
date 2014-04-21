/*
 * ProgramGrounder.cpp
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#include "ProgramGrounder.h"

#include <list>

void ProgramGrounder::ground() {
	statementDependency->createDependencyGraph(predicateTable);
	statementDependency->createComponentGraph();

	//Ground first rule
	groundRule(statementDependency->getRule(0));
}

void ProgramGrounder::groundRule(Rule* r) {

	//variable,value
	unordered_map<unsigned long, unsigned long> bound;
	list<unsigned long> id_match;


}

ProgramGrounder::~ProgramGrounder() {
	delete predicateTable;
	delete instancesTable;
	delete statementDependency;
	delete termsMap;

}

