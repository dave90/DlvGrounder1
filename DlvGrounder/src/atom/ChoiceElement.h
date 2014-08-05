/*
 * ChoiceElement.h
 *
 *  Created on: 04/ago/2014
 *      Author: tesi2
 */

#ifndef CHOICEELEMENT_H_
#define CHOICEELEMENT_H_

class Atom;

#include <vector>
#include "Atom.h"
using namespace std;

/// This class represents a choice element

class ChoiceElement {
public:
	//Default constructor
	ChoiceElement() {};

	///Getter for choice element
	const vector<Atom*>& getChoiceElement() const {return choiceElement;}
	///Setter for choice element
	void setChoiceElement(const vector<Atom*>& choiceElement) {this->choiceElement = choiceElement;}
	///This method returns the size of the vector of atoms
	const unsigned int getSize() const {return choiceElement.size();}
	/** This method returns the atom in the position i
	 * @retval a if the position i is valid, and a is the atom in that position
	 * @retval null if the position i is not valid
	 */
	Atom* getAtom(unsigned int i) const {if(i<choiceElement.size()) return choiceElement[i]; return 0;};

	///Destructor
	virtual ~ChoiceElement(){};

private:

	///Vector containing the atoms that compose the choice element
	/*
	 * Notice that the first atom must be a classical atom, while the remaining ones must be naf literals,
	 * so either classical literals or built-in atoms FIXME
	 */
	vector<Atom*> choiceElement;
};

#endif /* CHOICEELEMENT_H_ */
