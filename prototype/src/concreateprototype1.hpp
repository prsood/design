/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月19日
 */
#ifndef SRC_CONCREATEPROTOTYPE1_HPP_
#define SRC_CONCREATEPROTOTYPE1_HPP_
#include <iostream>
#include <string>
#include <vector>
#include "I_Prototype.hpp"
using namespace std;
/*
 *
 */
class concreateprototype1: public IPrototype
{
public:
    concreateprototype1();
    concreateprototype1(const concreateprototype1& _self);
    virtual ~concreateprototype1();
    vector<string> *GetNameList();
    IPrototype *clone() final;
    void InsetUser(string _name);
private:
    vector<string> nameList;
    int status;
};

#endif /* SRC_CONCREATEPROTOTYPE1_HPP_ */
