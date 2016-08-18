/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月17日
 */
#include "Black_Human.hpp"
#include <iostream>

using namespace std;
BlackHuman::BlackHuman()
{
    // TODO Auto-generated constructor stub

}

BlackHuman::~BlackHuman()
{
    // TODO Auto-generated destructor stub
    cout <<"Black man was killed" <<endl;
}

void BlackHuman::talk()
{
    cout << "I am a Black Man" << endl;
}

string BlackHuman::GetColor()
{
    return "Black";
}
