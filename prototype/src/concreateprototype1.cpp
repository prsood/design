/*
 * This file is part of BravoSystem.
 * Copyright(c) 2015 by bravovcloud.com
 * All rights reserved.
 *
 * Author
 *      - zouyeming <zouyeming@bravovcloud.com>
 * Create on 2016年8月19日
 */
#include "concreateprototype1.hpp"

concreateprototype1::concreateprototype1()
{
    // TODO Auto-generated constructor stub
    this->status = 0;
}

concreateprototype1::~concreateprototype1()
{
    // TODO Auto-generated destructor stub
    cout << "Prototype " <<status <<" Destory" <<endl;
}

concreateprototype1::concreateprototype1(const concreateprototype1& _self)
{
    cout<< "Copy Prototype" <<endl;
    this->status = _self.status + 1;
    this->nameList = _self.nameList;
}

IPrototype *concreateprototype1::clone()
{
    IPrototype *p;
    p = new concreateprototype1(*this);
//    vector<string> *s;
//    cout <<"size of origin " << sizeof(*this)<<endl;
//    cout <<"sizeof copy " << sizeof(*p)<<endl ;
//    s = p->GetNameList();
//    int count = this->nameList.size();
//    for(int i = 0 ;i < count; i++)
//        s->push_back(this->nameList[i]);
    return p;
}

vector<string> *concreateprototype1::GetNameList()
{
    return &(this->nameList);
}

void concreateprototype1::InsetUser(string _name)
{
    this->nameList.push_back(_name);
}
