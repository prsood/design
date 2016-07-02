/*
 * Director.h
 *
 *  Created on: 2016年7月2日
 *      Author: james
 */

#ifndef SRC_DIRECTOR_H_
#define SRC_DIRECTOR_H_
#include "Builder.h"
class Director {
public:
	Director();
	virtual ~Director();
	void Construct(Builder *builder);
};

#endif /* SRC_DIRECTOR_H_ */
