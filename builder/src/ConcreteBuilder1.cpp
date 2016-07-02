/*
 * ConcreteBuilder1.cpp
 *
 *  Created on: 2016年7月2日
 *      Author: james
 */

#include "ConcreteBuilder1.h"

ConcreteBuilder1::ConcreteBuilder1() {
	// TODO Auto-generated constructor stub

	product = new Product();
}

ConcreteBuilder1::~ConcreteBuilder1() {
	// TODO Auto-generated destructor stub
	if(product)
		delete product;
}

void ConcreteBuilder1::BuildPartA()
{
	product->Add("Part A");
}

void ConcreteBuilder1::BuildPartB()
{
	product->Add("Part B");
}

Product *ConcreteBuilder1::GetResult()
{
	return product;
}
