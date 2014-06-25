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

typedef unordered_map<index_object, index_object> map_index_object_index_object;


struct hashRule {
  size_t operator()(Rule* rule) const {
	  HashVecInt *hash=HashVecInt::getHashVecIntFromConfig();
	  vector<size_t> atomHash;
	  for(auto it=rule->getBeginHead();it!=rule->getEndHead();it++){
		  atomHash.push_back((*it)->getHash());
	  }
	  for(auto it=rule->getBeginBody();it!=rule->getEndBody();it++)
	  	  atomHash.push_back((*it)->getHash());
	  return hash->computeHashSize_T(atomHash);
  }
  bool operator()( Rule* r1,  Rule* r2)const{
	  return *r1==*r2;
  }

};

class GroundedRule{
public:
	GroundedRule(){}
	bool addRule(Rule*r)//{if(groundedRules.count(r))return false;groundedRules.insert(r);return true;};
	{
		if(!groundedRules.insert(r).second){delete r;return false;};
		return true;
	}
//	~GroundedRule(){for(auto rule:groundedRules)delete rule;}

private:
	unordered_set<Rule*,hashRule,hashRule> groundedRules;
	//FIXME hash no vector
//	vector<Rule*> groundedRules;
};


class ProgramGrounder {
public:
	ProgramGrounder(PredicateTable* pt,InstancesTable* it,StatementDependency* st):
		predicateTable(pt),instancesTable(it),statementDependency(st),termsMap(TermTable::getInstance()){};

	void ground();
	void groundRule(Rule *r);

	void print(){statementDependency->print(termsMap);};

	//Print Term
	void printTerm(){cout<<"Size Term table: "<<termsMap->getSize()<<endl;cout<<"Collision Term table: "<<termsMap->getCollision()<<endl;termsMap->print();	}
	void printFact(){instancesTable->print(termsMap);};

	virtual ~ProgramGrounder();
private:
	PredicateTable* predicateTable;
	InstancesTable* instancesTable;
	StatementDependency* statementDependency;
	TermTable* termsMap;
	GroundedRule groundedRule;



	/// Find bound and bind variable for each atom in rule
	/// @param r the rule
	/// @param bounds the vector to push the bound variable of atom in position i in rule
	/// @param binds the vector to push the bind variable of atom in position i in rule
	/// @param equal_vars is the map that indicate the term with the same variable
	void findBoundBindRule(Rule *r,vector<vec_pair_index_object> &bounds,vector<vec_pair_index_object>& binds,vector<map_int_int >& equal_vars);

	/// Print the gound rule
	void printGroundRule(Rule *r,map_index_object_index_object& var_assign);

	/// Set in bound the value for each variable present in current assignment
	void setBoundValue(Atom *current_atom,vec_pair_index_object &bound,map_index_object_index_object& var_assign);

	/// Remove the value in assignment for the variable present in bind
	void removeBindValueInAssignment(Atom *current_atom,vec_pair_index_object &bind,map_index_object_index_object& var_assign);
	/// Insert the value in assignment for the variable present in bind
	void insertBindValueInAssignment(Atom *current_atom,vec_pair_index_object &bind,map_index_object_index_object& var_assign);

	/// Debug methods
	void printVecPair(string name,vector<vec_pair_index_object> &vec);
	void printPair(int i, vector<vec_pair_index_object>& vec);
	void printMapIntInt(string name,vector<map_int_int >& equal_vars);
	void printAssignment(map_index_object_index_object& var_assign);
};



#endif /* PROGRAMGROUNDER_H_ */
