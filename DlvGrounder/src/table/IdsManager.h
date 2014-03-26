/*
 * IdsManager.h
 *
 *  Created on: 25/mar/2014
 *      Author: Jessica
 */

#ifndef IDSMANAGER_H_
#define IDSMANAGER_H_

#include <vector>
#include "IdManager.h"
using namespace std;

class IdsManager {
public:
	virtual ~IdsManager();
	static pair_long_bool getIndex(unsigned int i, string s);
	static const int TERM_ID_MANAGER=0;
	static const int PREDICATE_ID_MANAGER=1;
private:
	IdsManager();
	static vector<IdManager> idsManager;
};

#endif /* IDSMANAGER_H_ */
