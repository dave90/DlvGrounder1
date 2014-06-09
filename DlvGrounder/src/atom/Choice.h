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

	const vector<vector<index_object>> getChoiceElements() const {return choiceElements;};
	void setChoiceElements(const vector<vector<index_object> >& choiceElements) {this->choiceElements = choiceElements;};
	Binop getFirstBinop() const {return firstBinop;};
	void setFirstBinop(Binop firstBinop) {this->firstBinop = firstBinop;};
	index_object getFirstCompareTerm() const {return firstCompareTerm;};
	void setFirstCompareTerm(index_object firstCompareTerm) {this->firstCompareTerm = firstCompareTerm;};
	Binop getSecondBinop() const {return secondBinop;};
	void setSecondBinop(Binop secondBinop) {this->secondBinop = secondBinop;};
	index_object getSecondCompareTerm() const {return secondCompareTerm;};
	void setSecondCompareTerm(index_object secondCompareTerm) {this->secondCompareTerm = secondCompareTerm;};

	string getNameToHash();

	~Choice() {};

private:
	index_object firstCompareTerm;
	Binop firstBinop;
	index_object secondCompareTerm;
	Binop secondBinop;

	/* For each vector in choiceElements:
	 * the first element is the classical literal
	 * and the remaining elements are the naf literals
	 * */
	vector<vector<index_object>> choiceElements;

};

#endif /* CHOICE_H_ */
