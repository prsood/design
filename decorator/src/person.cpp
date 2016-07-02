/*
 * person.cpp
 *
 *  Created on: 2016年6月30日
 *      Author: james
 */

#include "person.h"

person::person(string name) {
	// TODO Auto-generated constructor stub
	this->name = name;

}

person::person()
{

}

person::~person() {
	// TODO Auto-generated destructor stub
}

void person::show()
{
	cout << name << endl;
}
