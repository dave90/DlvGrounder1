/*
 * Rule.h
 *
 *  Created on: 03/apr/2014
 *      Author: Jessica
 */

#ifndef RULE_H_
#define RULE_H_

#include <vector>
#include <unordered_set>
#include "../atom/Atom.h"

using namespace std;

class Rule {
public:
	Rule(){};

	const vector<Atom*>& getBody() const {return body;}
	void setBody(const vector<Atom*>& body) {this->body = body;}
	const vector<Atom*>& getHead() const {return head;}
	void setHead(const vector<Atom*>& head) {this->head = head;}

	bool isAStrongConstraint(){return head.empty();}
	bool isAFact(){return body.empty() && head.size()==1;}

	void addInHead(Atom* a){head.push_back(a);};
	void addInBody(Atom* a){body.push_back(a);};

	unordered_set<unsigned long> getPredicateInHead();
	unordered_set<unsigned long> getPredicateInBody();
	unordered_set<unsigned long> getPositivePredicateInBody();
	unordered_set<unsigned long> getNegativePredicateInBody();

	vector<Atom*>::const_iterator getBeginBody(){return body.begin();};
	vector<Atom*>::const_iterator getEndBody(){return body.end();};
	vector<Atom*>::const_iterator getBeginHead(){return head.begin();};
	vector<Atom*>::const_iterator getEndHead(){return head.end();};

	void clear(){head.clear();body.clear();};
	void print(TermTable*tb);

private:
	vector<Atom*> head;
	vector<Atom*> body;
};

#endif /* RULE_H_ */
