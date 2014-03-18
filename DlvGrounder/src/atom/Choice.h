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
	Choice(): firstCompareTerm(0), firstBinop(Binop::NONE_OP), secondCompareTerm(0), secondBinop(Binop::NONE_OP) {};

	const vector<vector<unsigned long>> getChoiceElements() const {return choiceElements;};
	void setChoiceElements(const vector<vector<unsigned long> >& choiceElements) {this->choiceElements = choiceElements;};
	Binop getFirstBinop() const {return firstBinop;};
	void setFirstBinop(Binop firstBinop) {this->firstBinop = firstBinop;};
	unsigned long getFirstCompareTerm() const {return firstCompareTerm;};
	void setFirstCompareTerm(unsigned long firstCompareTerm) {this->firstCompareTerm = firstCompareTerm;};
	Binop getSecondBinop() const {return secondBinop;};
	void setSecondBinop(Binop secondBinop) {this->secondBinop = secondBinop;};
	unsigned long getSecondCompareTerm() const {return secondCompareTerm;};
	void setSecondCompareTerm(unsigned long secondCompareTerm) {this->secondCompareTerm = secondCompareTerm;};

	string getNameToHash();

	~Choice() {};

private:
	unsigned long firstCompareTerm;
	Binop firstBinop;
	unsigned long secondCompareTerm;
	Binop secondBinop;

	/* For each vector in choiceElements:
	 * the first element is the classical literal
	 * and the remaining elements are the naf literals
	 * */
	vector<vector<unsigned long>> choiceElements;

};

#endif /* CHOICE_H_ */
