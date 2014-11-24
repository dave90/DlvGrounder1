/*
 * TermTable.h
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#ifndef TERMTABLE_H_
#define TERMTABLE_H_


#include "../term/Term.h"
#include "../utility/Config.h"
#include "Hashable.h"


using namespace std;


/*
 *  TermTable is an abstract class that represent a table of all Term in
 *  the logical program
 *
 *  TermTable is a Singleton
 */
class TermTable {
public:
	///Add term in the table and return the index
	virtual void addTerm(Term *&t){	terms.insert(t);}
	/// Get term
	virtual void getTerm(Term*& t){	terms.get(t);}
	/// Get size of the table
	virtual unsigned int getSize(){	return terms.flyweight_set.size();}
	/// Get the number of collision of the IdManager
	//TODO
	virtual int getCollision(){return 0;};

	// Method for fetch the instance of singleton
    // If termTable is null instance the table based on parameter in the configuration
	static TermTable* getInstance();
	virtual ~TermTable(){};
protected:
	// Instance of the singleton
	static TermTable* termTable;
private:
	TermTable(){};
	/*
	 *  Flyweight Factory of terms
	 */
	FlyweightIndexFactory<Term> terms;
};




#endif /* TERMTABLE_H_ */
