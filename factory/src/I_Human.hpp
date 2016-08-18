/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月17日
 */
#ifndef SRC_I_HUMAN_HPP_
#define SRC_I_HUMAN_HPP_
#include <string>
#include <iostream>
using namespace std;
/*
 *
 */
class IHuman
{
public:
    IHuman();
    virtual ~IHuman();
    virtual string GetColor() = 0;
    virtual void talk() = 0;
};

#endif /* SRC_I_HUMAN_HPP_ */
