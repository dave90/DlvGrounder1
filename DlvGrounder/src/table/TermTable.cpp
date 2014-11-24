/*
 * TermTable.cpp
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#include "TermTable.h"

TermTable *TermTable::termTable;

TermTable* TermTable::getInstance() {

	if (termTable == nullptr)
		termTable=new TermTable;

	return termTable;
}


