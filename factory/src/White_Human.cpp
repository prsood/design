/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月17日
 */
#include "White_Human.hpp"
#include <iostream>

using namespace std;
WhiteHuman::WhiteHuman()
{
    // TODO Auto-generated constructor stub

}

WhiteHuman::~WhiteHuman()
{
    // TODO Auto-generated destructor stub
    cout <<"White man was killed" <<endl;
}

void WhiteHuman::talk()
{
    cout << "I am a White Man" << endl;
}

string WhiteHuman::GetColor()
{
    return "White";
}
