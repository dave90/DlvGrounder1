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

#include <boost/unordered_set.hpp>
#include <unordered_set>


using namespace std;


/*
 *  TermTable is an abstract class that represent a table of all Term in
 *  the logical program
 *
 *  TermTable is a Singleton
 */
class TermTable {
public:
	TermTable(){};
	///Add term in the table and return the index
	virtual Term* addTerm(Term *t);
	/// Get term
	virtual Term* getTerm(Term* t);
	/// Get size of the table
	virtual unsigned int getSize();
	/// Get the number of collision of the IdManager
	virtual int getCollision();
	virtual ~TermTable();

	// Method for fetch the instance of singleton
    // If termTable is null instance the table based on parameter in the configuration
	static TermTable* getInstance();
protected:
	// Instance of the singleton
	static TermTable* termTable;

	/*
	 *  Flyweight Factory of terms
	 */
	FlyweightIndexFactory<Term> terms;
};




#endif /* TERMTABLE_H_ */
