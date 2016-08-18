/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月17日
 */
#ifndef SRC_ABSTRACT_FACTORY_HPP_
#define SRC_ABSTRACT_FACTORY_HPP_
#include <string>
#include <vector>
#include "I_Human.hpp"
using namespace std;
/*
 *
 */
class AbstractFactory
{
public:
    AbstractFactory();
    virtual ~AbstractFactory();
    virtual IHuman *CreateHuman(string color) = 0;
    virtual IHuman *GetHuman(string color) = 0;
    vector<IHuman*> Human;
};

#endif /* SRC_ABSTRACT_FACTORY_HPP_ */
