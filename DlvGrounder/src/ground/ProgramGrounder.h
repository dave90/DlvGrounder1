/*
 * ProgramGrounder.h
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#ifndef PROGRAMGROUNDER_H_
#define PROGRAMGROUNDER_H_

#include "../table/PredicateTable.h"
#include "../table/Instance.h"
#include "../table/TermTable.h"
#include "StatementDependency.h"

#include<vector>

using namespace std;

class ProgramGrounder {
public:
	ProgramGrounder(PredicateTable* pt,InstancesTable* it,StatementDependency* st,TermTable* tm):
		predicateTable(pt),instancesTable(it),statementDependency(st),termsMap(tm){};

	void ground(){statementDependency->createDependencyGraph(predicateTable);statementDependency->createComponentGraph();};

	void print(){statementDependency->print();};

	//Print Term
	void printTerm(){cout<<"Size Term table: "<<termsMap->getSize()<<endl;cout<<"Collision Term table: "<<termsMap->getCollision()<<endl;termsMap->print();	}


	virtual ~ProgramGrounder();
private:
	PredicateTable* predicateTable;
	InstancesTable* instancesTable;
	StatementDependency* statementDependency;
	TermTable* termsMap;
};

#endif /* PROGRAMGROUNDER_H_ */
