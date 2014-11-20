/*
 * Hashable.h
 *
 *  Created on: Nov 12, 2014
 *      Author: davide
 */

#ifndef HASHABLE_H_
#define HASHABLE_H_

#include <functional>
#include <unordered_set>

using namespace std;

class Hashable {
public:
	Hashable(){};
	virtual ~Hashable(){};

	virtual size_t hash()=0;
};

class Indexable{
public:
	Indexable():index(0){};
	Indexable(unsigned i):index(i){};
	virtual ~Indexable() {};

	unsigned getIndex() const {	return index;};
	void setIndex(unsigned index) {	this->index = index;};

private:
	unsigned index;
};

template<typename T>
struct HashForTable{
	inline size_t operator()(T* obj) const {
		return obj->hash();
	}

	inline bool operator()(T* obj1, T* obj2) const {
		return *obj1==*obj2;
	}
};

template<typename T>
struct ConstHashForTable{
	inline size_t operator()(const T& obj) const {
		return obj.hash();
	}

	inline bool operator()(const T& obj1,const T& obj2) const {
		return obj1==obj2;
	}
};

template<typename T>
class FlyweightFactory{
public:
	FlyweightFactory(){};
	~FlyweightFactory(){for(auto obj:flyweight_set)delete obj;};

	T* insert(T* obj){
		auto result=flyweight_set.insert(obj);
		if(!result.second)
			delete obj;
		return *result.first;
	}

	void print(){
		for(auto obj:flyweight_set)
			obj->print();
	}

private:
	unordered_set<T*,HashForTable<T>,HashForTable<T>> flyweight_set;
};


template<typename T>
class FlyweightIndexFactory{
public:
	FlyweightIndexFactory():index_counter(0){};
	~FlyweightIndexFactory(){for(auto obj:flyweight_set)delete obj;};

	T* insert(T* obj){
		auto result=flyweight_set.insert(obj);
		if(!result.second)
			delete obj;
		else{
			obj->setIndex(++index_counter);
		}
		return *result.first;
	}

	void print(){
		for(auto obj:flyweight_set)
			obj->print();
	}

private:
	unordered_set<T*,HashForTable<T>,HashForTable<T>> flyweight_set;
	unsigned int index_counter;
};

#endif /* HASHABLE_H_ */
