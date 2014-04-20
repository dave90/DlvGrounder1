/*
 * ProgramGrounder.cpp
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#include "ProgramGrounder.h"


void ProgramGrounder::ground(){
	statementDependency->createDependencyGraph(predicateTable);
	statementDependency->createComponentGraph();

	//Ground first rule
	groundRule(statementDependency->getRule(0));
}

void ProgramGrounder::groundRule(Rule* r) {


}

ProgramGrounder::~ProgramGrounder() {
	delete predicateTable;
	delete instancesTable;
	delete statementDependency;
	delete termsMap;

}

