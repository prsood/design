/*
 * main.cpp
 *
 *  Created on: 2016年6月30日
 *      Author: james
 */
#include "person.h"
#include "finery.h"
#include "Shoes.h"
#include "TShirt.h"
#include <iostream>

int main()
{
	person p("John");

	Shoes *sh = new Shoes();
	TShirt *ts = new TShirt();
	ts->Decorator(&p);
	sh->Decorator(ts);
	sh->show();
	return 0;
}



