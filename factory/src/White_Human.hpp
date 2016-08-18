/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月17日
 */
#ifndef SRC_WHITE_HUMAN_HPP_
#define SRC_WHITE_HUMAN_HPP_

#include "I_Human.hpp"

/*
 *
 */
class WhiteHuman: public IHuman
{
public:
    WhiteHuman();
    virtual ~WhiteHuman();
    string GetColor();
    void talk();
};

#endif /* SRC_WHITE_HUMAN_HPP_ */
