/*
 * Choice.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef CHOICE_H_
#define CHOICE_H_

#include "Atom.h"
#include <string>
using namespace std;

class Choice: public Atom {
public:
	///Default constructor
	Choice(): firstBinop(Binop::NONE_OP), secondBinop(Binop::NONE_OP) {};

	/** Constructor
	 * @param fB set the first binary operation
	 * @param sB set the second binary operation
	 * @param t set the terms vector
	 * @param cE set the choice elements vectors
	 */
	Choice(Binop fB, Binop sB, vector<Term*> t, vector<ChoiceElement> cE)
		: Atom(t), firstBinop(fB), secondBinop(sB), choiceElements(move(cE)) {};

	///Getter method for the first binary operation
	Binop getFirstBinop() const {return firstBinop;};
	///Setter method for the first binary operation
	void setFirstBinop(Binop firstBinop) {this->firstBinop = firstBinop;};
	///Getter method for the second binary operation
	Binop getSecondBinop() const {return secondBinop;};
	///Setter method for the second binary operation
	void setSecondBinop(Binop secondBinop) {this->secondBinop = secondBinop;};
	///Getter method for the choice elements
	const vector<ChoiceElement> getChoiceElements() const {return choiceElements;};
	///Setter method for the choice elements
	void setChoiceElements(const vector<ChoiceElement>& choiceElements) {this->choiceElements = choiceElements;};

	///This method compute the resulting hash of a choice atom TODO
	size_t getHash() const {return 0;};

	///Destructor
	~Choice() {};

private:
	///First binary operation
	Binop firstBinop;
	///Second binary operation
	Binop secondBinop;
	//Vector of choice elements
	vector<ChoiceElement> choiceElements;

	/* For the vector of terms, it contains the first and the second term of comparison.
	 * Notice that the vector contains the terms in the same order as they appear: the first term in position 0, the second in position 1.
	 */
};

#endif /* CHOICE_H_ */
