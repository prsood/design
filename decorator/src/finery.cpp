/*
 * finery.cpp
 *
 *  Created on: 2016年6月30日
 *      Author: james
 */

#include "finery.h"

finery::finery() {
	// TODO Auto-generated constructor stub
	componet = NULL;
}

finery::~finery() {
	// TODO Auto-generated destructor stub
}

void finery::Decorator(person *componet)
{
	this->componet = componet;

}

void finery::show()
{
	if( this->componet != NULL)
		this->componet->show();
}
