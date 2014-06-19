/*
 * Rule.h
 *
 *  Created on: 03/apr/2014
 *      Author: Jessica
 */

#ifndef RULE_H_
#define RULE_H_

#include <vector>
#include <set>
#include <unordered_set>
#include "../atom/Atom.h"
#include "../table/HashVecInt.h"

using namespace std;

struct atomCompare {
  bool operator() (const Atom* a1, const Atom* a2) const
  {
	  index_object p1=a1->getPredicate().second;
	  index_object p2=a2->getPredicate().second;
	  if(p1!=p2)return p1<p2;

	  return (a1->getHash())<(a2->getHash());
  }
};

typedef set<Atom*,atomCompare> OrderedAtomSet;

class Rule {
public:
	Rule(){};

	const OrderedAtomSet& getBody() const {return body;}
	void setBody(const OrderedAtomSet& body) {this->body = body;}
	const OrderedAtomSet& getHead() const {return head;}
	void setHead(const OrderedAtomSet& head) {this->head = head;}

	bool isAStrongConstraint(){return head.empty();}
	bool isAFact(){return body.empty() && head.size()==1;}

	void addInHead(Atom* a){head.insert(a);};
	void addInBody(Atom* a){body.insert(a);};

	unordered_set<index_object> getPredicateInHead();
	unordered_set<index_object> getPredicateInBody();
	unordered_set<index_object> getPositivePredicateInBody();
	unordered_set<index_object> getNegativePredicateInBody();
	unsigned int getSizeHead() const {return head.size();}
	unsigned int getSizeBody() const {return body.size();}

	OrderedAtomSet::const_iterator getBeginBody()const{return body.begin();};
	OrderedAtomSet::const_iterator getEndBody()const{return body.end();};
	OrderedAtomSet::const_iterator getBeginHead()const{return head.begin();};
	OrderedAtomSet::const_iterator getEndHead()const{return head.end();};

	void clear(){head.clear();body.clear();};
	void print(TermTable*tb);

	bool operator==(const Rule & r);

private:
	OrderedAtomSet head;
	OrderedAtomSet body;
};

#endif /* RULE_H_ */
