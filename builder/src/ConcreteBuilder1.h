/*
 * ConcreteBuilder1.h
 *
 *  Created on: 2016年7月2日
 *      Author: james
 */

#ifndef SRC_CONCRETEBUILDER1_H_
#define SRC_CONCRETEBUILDER1_H_
#include "Builder.h"
#include "Product.h"
class ConcreteBuilder1 : public Builder{
public:
	ConcreteBuilder1();
	virtual ~ConcreteBuilder1();
	void BuildPartA() ;
	void BuildPartB() ;
	Product *GetResult();
private:
	Product *product;
};

#endif /* SRC_CONCRETEBUILDER1_H_ */
