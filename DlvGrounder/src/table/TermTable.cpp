/*
 * TermTable.cpp
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#include "TermTable.h"

#include "BoostTermTable.h"
#include "HashTermTable.h"

TermTable *TermTable::termTable;

TermTable* TermTable::getInstance() {

	if (termTable == nullptr) {
		if (Config::getInstance()->getTermTableType() == TermTableType::BOOST)
			termTable = BoostTermTable::getInstance();
		else if (Config::getInstance()->getTermTableType()
				== TermTableType::STL)
			termTable = HashTermTable::getInstance();
		else
			termTable = HashTermTable::getInstance();
	}

	return termTable;
}
