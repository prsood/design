/*
 * Director.cpp
 *
 *  Created on: 2016年7月2日
 *      Author: james
 */

#include "Director.h"


Director::Director() {
	// TODO Auto-generated constructor stub

}

Director::~Director() {
	// TODO Auto-generated destructor stub
}

void Director::Construct(Builder * builder)
{
	builder->BuildPartA();
	builder->BuildPartB();
}
