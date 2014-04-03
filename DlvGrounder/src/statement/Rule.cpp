/*
 * Rule.cpp
 *
 *  Created on: 03/apr/2014
 *      Author: Jessica
 */

#include "Rule.h"
#include "iostream"

void  Rule::print(){
	for (unsigned int i = 0; i < head.size(); ++i) {
		head[i]->print();
		if(i!=head.size()-1)
			cout<<",";
	}
	cout<<" :- ";
	for (unsigned int i = 0; i < body.size(); ++i) {
		body[i]->print();
		if(i!=body.size()-1)
			cout<<",";
	}
	cout<<endl;
}
