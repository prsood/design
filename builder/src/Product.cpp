/*
 * Product.cpp
 *
 *  Created on: 2016年7月2日
 *      Author: james
 */

#include "Product.h"

Product::Product() {
	// TODO Auto-generated constructor stub

}

Product::~Product() {
	// TODO Auto-generated destructor stub
}

void Product::Add(string part)
{
	parts.push_back(part);
}

void Product::show()
{
	cout <<"Create product----" <<endl;
	list<string>::iterator it;
	for(it = parts.begin();it!=parts.end();it++)
		cout <<"\t " << *it << endl;
}
