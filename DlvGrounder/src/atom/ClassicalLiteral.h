/*
 * ClassicalLiteral.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#ifndef CLASSICALLITERAL_H_
#define CLASSICALLITERAL_H_

#include <vector>
#include "Atom.h"
using namespace std;

class ClassicalLiteral : public Atom {
public:
	ClassicalLiteral(): predicate(0), hasMinus(false), negative(false){};
	ClassicalLiteral(index_object p, vector<index_object> &t, bool hM, bool n): predicate(p),terms(move(t)), hasMinus(hM), negative(n) {};

	vector<index_object>& getTerms() {return terms;};
	virtual const unsigned int getTermsSize() const {return terms.size();};
	virtual index_object getTerm(int i) const{return terms[i];};
	void setTerms(const vector<index_object>& terms){this->terms=terms;};
	pair<bool,index_object> getPredicate() const {return {true,predicate};};
	void setPredicate(index_object predicate) {this->predicate=predicate;};
	bool isHasMinus() const {return hasMinus;};
	void setHasMinus(bool hasMinus) {this->hasMinus=hasMinus;};
	bool isNegative() const {return negative;};
	void setNegative(bool negative) {this->negative=negative;};

	size_t getHash() const;

	/// Compare predicate and terms of atoms, because used for fact
	bool operator==(const Atom& a);

	///Print Method , static because instance not have atoms but vector of terms
	static void print(index_object predicate,vector<index_object>& terms,bool negative,bool hasMinus);
	void print();


	//TODO metodi accesso lista termini
	~ClassicalLiteral() {};

private:
	index_object predicate;
	vector<index_object> terms;
	bool hasMinus;
	bool negative;
};

#endif /* CLASSICALLITERAL_H_ */
