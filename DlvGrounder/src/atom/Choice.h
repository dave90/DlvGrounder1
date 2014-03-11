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
#include <vector>
using namespace std;

class Choice: public Atom {
public:
	Choice(): firstCompareTerm(0), secondCompareTerm(0) {};

	const string& getFirstBinop() const;
	void setFirstBinop(const string& firstBinop);
	unsigned long getFirstCompareTerm() const;
	void setFirstCompareTerm(unsigned long firstCompareTerm);
	const string& getSecondBinop() const;
	void setSecondBinop(const string& secondBinop);
	unsigned long getSecondCompareTerm() const;
	void setSecondCompareTerm(unsigned long secondCompareTerm);
	const vector<vector<unsigned long> >& getChoiceElements() const;
	void setChoiceElements(const vector<vector<unsigned long> >& choiceElements);

	virtual ~Choice();

private:
	unsigned long firstCompareTerm;
	string firstBinop;
	unsigned long secondCompareTerm;
	string secondBinop;

	/* For each vector in choiceElements:
	 * the first element is the classical literal
	 * and the remaining elements are the naf literals
	 * */
	vector<vector<unsigned long>> choiceElements;

};

#endif /* CHOICE_H_ */
