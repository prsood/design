/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月17日
 */
#ifndef SRC_BLACK_HUMAN_HPP_
#define SRC_BLACK_HUMAN_HPP_

#include "I_Human.hpp"

/*
 *
 */
class BlackHuman: public IHuman
{
public:
    BlackHuman();
    virtual ~BlackHuman();
    string GetColor();
    void talk();
};

#endif /* SRC_BLACK_HUMAN_HPP_ */
