/*
 * finery.h
 *
 *  Created on: 2016年6月30日
 *      Author: james
 */

#ifndef SRC_FINERY_H_
#define SRC_FINERY_H_

#include "person.h"

class finery: public person {
public:
	finery();
	virtual ~finery();
	void Decorator(person *componet);
	virtual void show();
protected:
	person *componet;

};

#endif /* SRC_FINERY_H_ */
