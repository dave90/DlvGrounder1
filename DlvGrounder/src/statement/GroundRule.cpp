/*
 * GroundRule.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: david
 */

#include "GroundRule.h"

bool GroundRule::operator ==(const GroundRule& r) {
	  if(getSizeBody()!=r.getSizeBody())return false;
	  if(getSizeHead()!=r.getSizeHead())return false;
	  for(auto atom:head){
		  bool find=false;
		  for(auto it=r.getBeginHead();it!=r.getEndHead();it++)
			  if(*atom==*(*it)){
				  find=true;
				  break;
			  }
		  if(!find)return false;
	  }
	  for(auto atom:body){
		  bool find=false;
		  for(auto it=r.getBeginBody();it!=r.getEndBody();it++)
			  if(*atom==*(*it)){
				  find=true;
				  break;
			  }
		  if(!find)return false;
	  }

	  return true;
}

void GroundRule::print() {
	if(head.size()>0){
		unsigned int i=0;
		for (auto atom:head) {
			ClassicalLiteral::print(atom->predicate,atom->atom->terms,false,false);
			if(i!=head.size()-1)
				cout<<";";
			i++;
		}
		if(body.size()==0) { cout<<"."<<endl; return; }
	}
	cout<<":-";
	unsigned int i=0;
	for (auto atom:body) {
		ClassicalLiteral::print(atom->predicate,atom->atom->terms,false,false);
		cout<<i<<endl;
		cout<<body.size()-1<<endl;
		if(i<body.size()-1)
			cout<<";";
		i++;
	}
	cout<<"."<<endl;
}

GroundRule::~GroundRule() {
	for(auto atom:head)delete atom;
	for(auto atom:body)delete atom;
}

