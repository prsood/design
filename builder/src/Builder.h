/*
 * Builder.h
 *
 *  Created on: 2016年7月2日
 *      Author: james
 */

#ifndef SRC_BUILDER_H_
#define SRC_BUILDER_H_
#include "Product.h"
class Builder {
public:
	Builder();
	virtual ~Builder();
	virtual void BuildPartA() = 0;
	virtual void BuildPartB() = 0;
	virtual Product *GetResult() = 0;
};

#endif /* SRC_BUILDER_H_ */
