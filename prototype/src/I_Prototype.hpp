/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月19日
 */
#ifndef SRC_I_PROTOTYPE_HPP_
#define SRC_I_PROTOTYPE_HPP_
#include <iostream>
#include <string>
#include <vector>
using namespace std;
/*
 *
 */
class IPrototype
{
public:
    IPrototype();
    virtual ~IPrototype();
    virtual IPrototype *clone() = 0;
    virtual void InsetUser(string _name) = 0;
    virtual vector<string> *GetNameList() = 0;
};

#endif /* SRC_I_PROTOTYPE_HPP_ */
