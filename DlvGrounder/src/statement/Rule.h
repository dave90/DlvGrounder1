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
#include "../table/HashVecInt.h"

using namespace std;

/**
 * @brief This class represents a rule with its body and head atoms
 */

class Rule {
public:
	///Default constructor
	Rule(){};

	///Getter method for body atoms
	const vector<Atom*>& getBody() const {return body;}
	///Setter method for body atoms
	void setBody(const vector<Atom*>& body) {this->body = body;}
	///Getter method for head atoms
	const vector<Atom*>& getHead() const {return head;}
	///Setter method for head atoms
	void setHead(const vector<Atom*>& head) {this->head = head;}

	///This method returns true if it is a strong constrain
	bool isAStrongConstraint(){return head.empty();}
	///This method returns true if it is a fact
	bool isAFact(){return body.empty() && head.size()==1;}

	///This method adds an atom in the head
	void addInHead(Atom* a){head.push_back(a);};
	///This method adds an atom in the body
	void addInBody(Atom* a){body.push_back(a);};

	///This method returns the set of predicate in the head
	unordered_set<index_object> getPredicateInHead();
	///This method returns the set of predicate in the body
	unordered_set<index_object> getPredicateInBody();
	///This method returns the set of predicate of just the positive atoms in the body
	unordered_set<index_object> getPositivePredicateInBody();
	///This method returns the set of predicate of just the negative atoms in the body
	unordered_set<index_object> getNegativePredicateInBody();
	///This method returns the size of the head
	unsigned int getSizeHead() const {return head.size();}
	///This method return the size of the body
	unsigned int getSizeBody() const {return body.size();}

	///This method returns an iterator that points to the first atom in the body
	vector<Atom*>::const_iterator getBeginBody()const{return body.begin();};
	///This method returns an iterator that points to the last atom in the body
	vector<Atom*>::const_iterator getEndBody()const{return body.end();};
	///This method returns an iterator that points to the first atom in the head
	vector<Atom*>::const_iterator getBeginHead()const{return head.begin();};
	///This method returns an iterator that points to the last atom in the head
	vector<Atom*>::const_iterator getEndHead()const{return head.end();};

	///This method remove all the atoms in the body and in the head
	void clear(){head.clear();body.clear();};

	///Printer method
	void print();

	/** @brief Equal-to operator for rules
	 *  @details Two rules are equal if they have the same atoms in the body and in the head regardless the order in which they appear
	 */
	bool operator==(const Rule & r);

	///Destructor
	~Rule();

private:
	///Vector of the atoms in head
	vector<Atom*> head;
	///Vector of the atoms in body
	vector<Atom*> body;
};

#endif /* RULE_H_ */
