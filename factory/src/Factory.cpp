/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月17日
 */
#include "Factory.hpp"
#include "Black_Human.hpp"
#include "White_Human.hpp"
Factory::Factory()
{
    // TODO Auto-generated constructor stub

}

Factory::~Factory()
{
    // TODO Auto-generated destructor stub
    IHuman *p;
    for(int i = 0;i<Human.size();i++)
    {
        cout <<"Shoot ====> ";
        p = Human[i];
        delete p;
        p = NULL;
    }
}

IHuman *Factory::GetHuman(string color)
{
    auto it = Human.begin();
    for(;it!= Human.end();it++)
    {
        if(color == (*it)->GetColor())
            return *it;
    }
    return NULL;
}
IHuman *Factory::CreateHuman(string color)
{
    IHuman *_human;
    if (color =="Black")
    {
        if((_human = GetHuman(color)) == NULL)
        {
            cout << "Create a Black man" <<endl;
            _human = new BlackHuman();
            Human.push_back(_human);
        }
            return _human;
    }
    if (color =="White")
    {
        if((_human = GetHuman(color)) == NULL)
        {
            cout << "Create a White man" <<endl;
            _human = new WhiteHuman();
            Human.push_back(_human);
        }
        return _human;
    }
    return NULL;
}

int Factory::GetHumanCount()
{
    return Human.size();
}
