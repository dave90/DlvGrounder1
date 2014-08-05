/*
 * AggregateElement.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef AGGREGATEELEMENT_H_
#define AGGREGATEELEMENT_H_

class Atom;

#include <vector>
#include "../utility/IndexDefinition.h"
#include "Atom.h"
using namespace std;

/// This class represents an aggregate element
class AggregateElement {

	public:
		//Default constructor
		AggregateElement() {};

		///Getter for terms
		const vector<index_object>& getTerms() const {return terms;}
		///Setter for terms
		void setTerms(const vector<index_object>& terms) {this->terms = terms;}
		///Getter for naf literals
		const vector<Atom*>& getNafLiterals() const {return nafLiterals;}
		///Setter for naf literals
		void setNafLiterals(const vector<Atom*>& nafLiterals) {this->nafLiterals = nafLiterals;}

		//Destructor
		virtual ~AggregateElement() {};

	private:
		///Vector of terms
		vector<index_object> terms;
		///Vector of naf literals
		// Notice that the atoms must be naf literals, so either classical literals or built-in atoms FIXME
		vector<Atom*> nafLiterals;
};

#endif /* AGGREGATEELEMENT_H_ */
