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

typedef unordered_map<unsigned long, unsigned long> map_long_long;




class ProgramGrounder {
public:
	ProgramGrounder(PredicateTable* pt,InstancesTable* it,StatementDependency* st,TermTable* tm):
		predicateTable(pt),instancesTable(it),statementDependency(st),termsMap(tm){};

	void ground();
	void groundRule(Rule *r);
	void groundJoinRule(Rule *r);

	void print(){statementDependency->print(termsMap);};

	//Print Term
	void printTerm(){cout<<"Size Term table: "<<termsMap->getSize()<<endl;cout<<"Collision Term table: "<<termsMap->getCollision()<<endl;termsMap->print();	}


	virtual ~ProgramGrounder();
private:
	PredicateTable* predicateTable;
	InstancesTable* instancesTable;
	StatementDependency* statementDependency;
	TermTable* termsMap;


	/// Find bound and bind variable for each atom in rule
	/// @param r the rule
	/// @param bounds the vector to push the bound variable of atom in position i in rule
	/// @param binds the vector to push the bind variable of atom in position i in rule
	void findBoundBindRule(Rule *r,vector<vec_pair_long> &bounds,vector<vec_pair_long>& binds);

	/// Method called when find the assignment in ground rule
	void foundAssignmentRule(Rule *r,map_long_long& var_assign);

	/// Set in bound the value for each variable present in current assignment
	void setBoundValue(Atom *current_atom,vec_pair_long &bound,map_long_long& var_assign);

	/// Remove the value in assignment for the variable present in bind
	void removeBindValueInAssignment(Atom *current_atom,vec_pair_long &bind,map_long_long& var_assign);
	/// Insert the value in assignment for the variable present in bind
	void insertBindValueInAssignment(Atom *current_atom,vec_pair_long &bind,map_long_long& var_assign);

	/// JOIN METHOD
	void getBindVariable(Atom *a,vector<unsigned long>& bind);
	void getCommonVariablesIndex(Atom *a1,Atom *a2,vector<unsigned long>& common1,vector<unsigned long>& common2);

};
#endif /* PROGRAMGROUNDER_H_ */
