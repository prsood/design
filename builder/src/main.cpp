/*
 * main.cpp
 *
 *  Created on: 2016年7月2日
 *      Author: james
 */

#include <iostream>
#include "Director.h"
#include "Product.h"
#include "ConcreteBuilder1.h"
#include "ConcreteBuilder2.h"
int main()
{
	Director director;
	Builder *b1 = new ConcreteBuilder1();
	Builder *b2 = new ConcreteBuilder2();

	director.Construct(b1);
	Product *p1 = b1->GetResult();
	p1->show();

	director.Construct(b2);
	Product *p2 = b2->GetResult();
	p2->show();
	delete b1;
	delete b2;
}


