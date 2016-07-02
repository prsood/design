/*
 * ConcreteBuilder2.h
 *
 *  Created on: 2016年7月2日
 *      Author: james
 */

#ifndef SRC_CONCRETEBUILDER2_H_
#define SRC_CONCRETEBUILDER2_H_
#include "Product.h"
#include "Builder.h"

class ConcreteBuilder2 : public Builder{
public:
	ConcreteBuilder2();
	virtual ~ConcreteBuilder2();
	void BuildPartA() ;
	void BuildPartB() ;
	Product *GetResult();
private:
	Product *product;
};

#endif /* SRC_CONCRETEBUILDER2_H_ */
