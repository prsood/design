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
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void printVector(vector<string> *s)
{
    auto it = s->begin();
    for (; it != s->end(); it++)
        cout << *it <<" ";
    cout << endl;
}
int main()
{
    IPrototype *original = new concreateprototype1();
    IPrototype *copy1 = NULL;
    IPrototype *copy2 = NULL;
    original->InsetUser("Alice");
    copy1 = original->clone();
    copy1->InsetUser("Bob");
    copy2 = copy1->clone();
    copy2->InsetUser("James");
    cout << "original list:";
    printVector(original->GetNameList());
    cout << "original copy1 list:";
    printVector(copy1->GetNameList());
    cout << "original copy2 list:";
    printVector(copy2->GetNameList());
    delete original;
    delete copy1;
    delete copy2;
    return 0;
}

