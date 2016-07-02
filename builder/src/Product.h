/*
 * Product.h
 *
 *  Created on: 2016年7月2日
 *      Author: james
 */

#ifndef SRC_PRODUCT_H_
#define SRC_PRODUCT_H_
#include <iostream>
#include <string>
#include <list>

using namespace std;

class Product {
public:
	Product();
	virtual ~Product();
	void Add(string part);
	void show();
private:
	list <string> parts;
};

#endif /* SRC_PRODUCT_H_ */
