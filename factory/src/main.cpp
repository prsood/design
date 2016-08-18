/*
* This file is part of BravoSystem.
* Copyright(c) 2015 by bravovcloud.com
* All rights reserved.
*
* Author
*      - zouyeming <zouyeming@bravovcloud.com>
* Create on 2016年8月17日
*/
#include <iostream>
#include "Factory.hpp"
#include "Black_Human.hpp"
#include "I_Human.hpp"
int main()
{
    IHuman *black = NULL;
    IHuman *white = NULL;
    Factory factory;

    black = factory.CreateHuman("Black");
    black->talk();
    cout << "Human count: " << factory.GetHumanCount() <<endl;
    white = factory.CreateHuman("White");
    white->talk();
    cout << "Human count: " << factory.GetHumanCount() <<endl;
    white = factory.CreateHuman("White");
    white->talk();
    cout << "Human count: " << factory.GetHumanCount() <<endl;
    return 0;
}

