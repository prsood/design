/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月17日
 */
#ifndef SRC_FACTORY_HPP_
#define SRC_FACTORY_HPP_

#include "Abstract_Factory.hpp"

/*
 *
 */
class Factory: public AbstractFactory
{
public:
    Factory();
    virtual ~Factory();
    IHuman *CreateHuman(string color);
    int GetHumanCount();
private:
    IHuman *GetHuman(string color);
};

#endif /* SRC_FACTORY_HPP_ */
