/*
 * person.h
 *
 *  Created on: 2016年6月30日
 *      Author: james
 */

#ifndef SRC_PERSON_H_
#define SRC_PERSON_H_
#include <string>
#include <iostream>
using namespace std;

class person {
public:
	person(string name);
	person();
	virtual ~person();
	virtual void show();
private:
	string name;
};

#endif /* SRC_PERSON_H_ */
