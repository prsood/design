/*
 * ConcreteBuilder2.cpp
 *
 *  Created on: 2016年7月2日
 *      Author: james
 */

#include "ConcreteBuilder2.h"

ConcreteBuilder2::ConcreteBuilder2() {
	// TODO Auto-generated constructor stub

	product = new Product();
}

ConcreteBuilder2::~ConcreteBuilder2() {
	// TODO Auto-generated destructor stub
	if(product)
		delete product;
}

void ConcreteBuilder2::BuildPartA()
{
	product->Add("Part X");
}

void ConcreteBuilder2::BuildPartB()
{
	product->Add("Part Y");
}

Product *ConcreteBuilder2::GetResult()
{
	return product;
}
